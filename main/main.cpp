#include <Arduino.h>

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h> // alias for `#include "SSD1306Wire.h"`
#include <OLEDDisplayUi.h>
#include <esp_log.h>
static const char* LOG_TAG = "Espers";

#include <BLE.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define BLE
BLE ble;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "0d531e57-d84e-4b12-830e-54acbefc22b1"
#define HEARTRATE_CHARACTERISTIC_UUID "8db3c4cc-8279-457d-9514-11127f8ae75a"
#define MSG_CHARACTERISTIC_UUID "8db3c5cc-8279-457d-9514-11127f8ae75a"
#define CTRL_CHARACTERISTIC_UUID "63b9c4cc-8279-458d-9514-11127f8ae75a"
BLECharacteristic* pHeartChara;
BLECharacteristic* pCtrlChara;
std::string serviceMessage="unused";

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 5 , 4);
OLEDDisplayUi ui(&display);
#define ADCMAX (1<<12)
int sig1;


#define HEARTPIN 0
int heartrate = 0;
float signalBuffer[DISPLAY_WIDTH];
int signalBufferOffset=0;

#define storeSignal(v) signalBuffer[signalBufferOffset++%DISPLAY_WIDTH]=(v)

typedef enum { DISPLAY_HOME, DISPLAY_SIGNAL, DISPLAY_PROGRESS} DisplayState;
DisplayState dispState = DISPLAY_SIGNAL;

void updateOLED(){
  if(1){ // Invert colors
    display.clear();
    display.setColor(WHITE);
  }else{
    display.setColor(WHITE);
    display.fillRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
    display.setColor(BLACK);
  }

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  switch(dispState){
    case DISPLAY_HOME:
      display.drawString(0,0,"HOME");
      break;
    case DISPLAY_SIGNAL:
      // Draw midline.
      for(int i = 0; i< DISPLAY_WIDTH; i++) display.setPixel(i,DISPLAY_HEIGHT/2);

      if(0){ // Visualize signal as scattered pixels
        for(int i = 0; i< DISPLAY_WIDTH; i++){
          display.setPixel(i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH]);
        }
      }else{ // Visualize signal as connected lines
        for (size_t i = 1; i < DISPLAY_WIDTH; i++) {
          float y0 = signalBuffer[(signalBufferOffset+i-1)%DISPLAY_WIDTH];
          float y1 = signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH];
          display.drawLine(i-1,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y0,i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y1);
        }
      }
      // Display raw value in top left
      display.drawString(0,0,String("SIGNAL "+ String(sig1,DEC)));
      // Display blemessage

      display.drawString(0,12,serviceMessage.c_str());
      // Temporary display for heartrate.
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(DISPLAY_WIDTH,0,String(heartrate));
      break;
    case DISPLAY_PROGRESS:
      int progress = 50 % 100;
      // draw the progress bar
      display.drawProgressBar(0, 32, 120, 10, progress);

      // draw the percentage as String
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 15, String(progress) + "%");
      break;
  }
  if(0){
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(DISPLAY_WIDTH, DISPLAY_HEIGHT-10, String(millis()));
  }
  display.display();
}
int period=0;
int nPulses=0;
bool released=true;
int prevHeart=0;

void calculateHeartrate(){
  if(sig1 > (ADCMAX>>1)+200){
    if(released){
      nPulses++;
      released=false;
    }
  }else{
    released=true;
  }
  if(period + 3000 < millis()){
    prevHeart=heartrate;
    heartrate = (nPulses*20+prevHeart)/2;
    period=millis();
    nPulses=0;
    pHeartChara->setValue(String(heartrate).c_str());
    pHeartChara->notify();
  }

}

uint8_t globDelay=20;
void handleSerial(){
  if(Serial.available()>1){
    if(0){
      String line = Serial.readStringUntil('\n');
      line.trim();

      size_t pos=line.indexOf(' ');
      String code = line.substring(0,pos);
      String argss = line.substring(pos);
      argss.trim();
      Serial.println(String("Code: '"+code+"' Args: `"+argss+"`"));
      if(code.equalsIgnoreCase("S0")){

      }
    }else{
      char code = Serial.read();
      switch(code){
        case 'S':
        case 's':
          dispState= (DisplayState) Serial.parseInt();
          break;
      }
    }
  }
}

/* --------- Initialize BLE --- */
class TestCb : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic){
    pCharacteristic->setValue(serviceMessage);
  }
  void onWrite(BLECharacteristic *pCharacteristic){
    serviceMessage = pCharacteristic->getValue(); 
  }
};

class CtrlCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic){
  }
  void onWrite(BLECharacteristic *pCharacteristic){
    std::string cmd = pCharacteristic->getValue();
    switch(cmd[0]){
      case 0:
        dispState = (DisplayState) cmd[1];  
        break;
    }
  }
};
void initBLE(){
  ESP_LOGD(LOG_TAG, ">> BLE initialization starting.");
  // Initalize BLE env
  BLE::initServer("Espers");

  // Create BLE Server
  BLEServer* pServer = new BLEServer();

  // Create the service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create the characteristics
  pHeartChara = pService->createCharacteristic(
      HEARTRATE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ
      );
  pHeartChara->setValue("<initializing>");
  // Message/display characteristic
  BLECharacteristic* pMsgChr = pService->createCharacteristic(
      MSG_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE
      );
  pMsgChr->setCallbacks(new TestCb());

  pCtrlChara = pService->createCharacteristic(
      CTRL_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE
      );
  pCtrlChara->setCallbacks(new CtrlCallbacks());


  // Start the service (now it's usable but not advertising it's existance)
  pService->start();

  // Start advertising this server.
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  ESP_LOGD(LOG_TAG, ">> BLE initialization finished.");
}

/* --------- Setup & Loop --- */
void setup() {
  ESP_LOGD(LOG_TAG, ">> Starting up");
  Serial.begin(115200);
  ESP_LOGD(LOG_TAG, ">> Initializing display");
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  pinMode(HEARTPIN,INPUT);
  for(int i=0;i<DISPLAY_WIDTH;i++){
    storeSignal(0.7);
  }
  initBLE();
}
uint16_t lastNotify=0;
#define NOTIFY_INTERVAL 1000
void loop() {
  sig1=analogRead(HEARTPIN);
  storeSignal(sig1/4095.f);
  handleSerial();
  calculateHeartrate();
  updateOLED();
  if(millis() > lastNotify + NOTIFY_INTERVAL){

    lastNotify = millis(); 
  }
  delay(globDelay );
}

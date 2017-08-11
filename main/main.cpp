#include <Arduino.h>

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "oled/SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "oled/OLEDDisplayUi.h"
// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 5 , 4);
OLEDDisplayUi ui(&display);

#define ADCMAX (1<<12)
int sig1;
#define HEARTPIN 25
int heartrate = 0;
float signalBuffer[DISPLAY_WIDTH];
int signalBufferOffset=0;

#define storeSignal(v) signalBuffer[signalBufferOffset++%DISPLAY_WIDTH]=(v)

void setup() {
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  pinMode(HEARTPIN,INPUT);
  for(int i=0;i<DISPLAY_WIDTH;i++){
    storeSignal(0.7);
  }
}

typedef enum { DISPLAY_HOME, DISPLAY_SIGNAL, DISPLAY_PROGRESS} DisplayState;
DisplayState dispState = DISPLAY_SIGNAL;
void updateOLED(){
  #if 0 // Invert colors
    display.clear();
    display.setColor(WHITE);
  #else
    display.setColor(WHITE);
    display.fillRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
    display.setColor(BLACK);
  #endif
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  switch(dispState){
    case DISPLAY_HOME:
      display.drawString(0,0,"HOME");
      break;
    case DISPLAY_SIGNAL:
      // Draw midline.
      #define SIG_MIDLINE 1
      #ifdef SIG_MIDLINE
        for(int i = 0; i< DISPLAY_WIDTH; i++) display.setPixel(i,DISPLAY_HEIGHT/2);
      #endif

      #if 0 // Visualize signal as scattered pixels
        for(int i = 0; i< DISPLAY_WIDTH; i++){
          display.setPixel(i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH]);
        }
      #else // Visualize signal as connected lines
        for (size_t i = 1; i < DISPLAY_WIDTH; i++) {
          float y0 = signalBuffer[(signalBufferOffset+i-1)%DISPLAY_WIDTH];
          float y1 = signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH];
          display.drawLine(i-1,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y0,i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y1);
        }
      #endif
      // Display raw value in top left
      display.drawString(0,0,String("SIGNAL "+ String(sig1,DEC)));
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
  #if 0
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(90, 10, String(millis()));
  #endif
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
  }

}
uint8_t globDelay=5;
void handleSerial(){
  if(Serial.available()>1){
    #if 0
    String line = Serial.readStringUntil('\n');
    line.trim();
    size_t pos=line.indexOf(' ');
    String code = line.substring(0,pos);
    String argss = line.substring(pos);
    argss.trim();
    Serial.println(String("Code: '"+code+"' Args: `"+argss+"`"));
    if(code.equalsIgnoreCase("S0")){

    }
    #else
      char code = Serial.read();
      switch(code){
        case 'S':
        case 's':
          dispState= (DisplayState) Serial.parseInt();
          break;
      }
    #endif
  }
}
void loop() {
  sig1=analogRead(HEARTPIN);
  storeSignal(sig1/4095.f);
  handleSerial();
  calculateHeartrate();
  updateOLED();
  delay(globDelay);
}

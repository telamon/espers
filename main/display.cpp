
#include "display.h"

namespace espers {
  Display::Display(){
    // Initialize the OLED display using Wire library
    display = new SSD1306(I2C_ADDR, I2C_SDA, I2C_SCL);
    display->init();
    display->flipScreenVertically();
    display->setContrast(255);
    dispState = DISPLAY_SIGNAL;
  }
  void Display::storeSignal(float normalizedSample){
    //#define storeSignal(v) signalBuffer[signalBufferOffset++%DISPLAY_WIDTH]=(v)
    signalBuffer[signalBufferOffset++%DISPLAY_WIDTH]=normalizedSample;
  }
  void Display::setSig1(uint16_t sample){
    sig1 = sample;
  }

  DisplayState Display::getState(){
    return dispState;
  }
  
  void Display::setState(DisplayState state){
    dispState = state;
  }
  void Display::setProgress(uint8_t percentage){
    this->progress = percentage % 100;
  }
  void Display::smoothProgressFinish(uint8_t frameDelay){
    while(progress<101){
      progress++;
      redraw(millis());
      delay(frameDelay);
    }
  }
  void Display::redraw(uint32_t millis){
    if(1){ // Invert colors
      display->clear();
      display->setColor(WHITE);
    }else{
      display->setColor(WHITE);
      display->fillRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
      display->setColor(BLACK);
    }

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    switch(dispState){
      case DISPLAY_HOME:
        display->drawString(0,0,"HOME");
        break;
      case DISPLAY_SIGNAL:
        // Draw midline.
        for(int i = 0; i< DISPLAY_WIDTH; i++) display->setPixel(i,DISPLAY_HEIGHT/2);

        if(0){ // Visualize signal as scattered pixels
          for(int i = 0; i< DISPLAY_WIDTH; i++){
            display->setPixel(i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH]);
          }
        }else{ // Visualize signal as connected lines
          for (size_t i = 1; i < DISPLAY_WIDTH; i++) {
            float y0 = signalBuffer[(signalBufferOffset+i-1)%DISPLAY_WIDTH];
            float y1 = signalBuffer[(signalBufferOffset+i)%DISPLAY_WIDTH];
            display->drawLine(i-1,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y0,i,DISPLAY_HEIGHT- (DISPLAY_HEIGHT/1.0f)*y1);
          }
        }
        // Display raw value in top left
        display->drawString(0,0,String("SIGNAL "+ String(sig1,DEC)));
        // Display blemessage

        display->drawString(0,12,serviceMessage.c_str());
        // Temporary display for heartrate.
        display->setTextAlignment(TEXT_ALIGN_RIGHT);
        //display->drawString(DISPLAY_WIDTH,0,String(heartrate));
        break;
      case DISPLAY_PROGRESS:
        // draw the progress bar
        display->drawProgressBar(0, 32, 120, 10, progress);

        // draw the percentage as String
        display->setTextAlignment(TEXT_ALIGN_CENTER);
        display->drawString(64, 15, String(progress) + "%");
        break;
    }
    if(0){
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(DISPLAY_WIDTH, DISPLAY_HEIGHT-10, String(millis));
    }
    display->display();
  }

} // namespace espers

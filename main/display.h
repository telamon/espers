/*
 * Manages the on-board oled display
 */
#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <Arduino.h>
// Include the correct display library
// For a connection via I2C using Wire include
#include <SSD1306.h>  // alias for `#include "SSD1306Wire.h"`
#include <Wire.h>     // Only needed for Arduino 1.6.5 and earlier
#include "application_state.h"
#include "settings.h"
namespace espers {
#define I2C_ADDR (0x3c)
#define I2C_SDA (5)
#define I2C_SCL (4)

class Display {
 public:
  Display(ApplicationState* pState);
  void redraw();
  // The display can render signals normalized to values between 0..1
  void storeSignal(float normalizedSample);
  void setProgress(uint8_t percentage);
  void animateProgress(uint8_t percentage, uint8_t frameDelay);

 private:
  SSD1306* display;
  float signalBuffer[DISPLAY_WIDTH];
  uint16_t signalBufferOffset = 0;
  uint8_t progress;
  ApplicationState* pState;
  void drawHome();
};

}  // namespace espers

#endif

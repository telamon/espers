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

namespace espers {
#define I2C_ADDR (0x3c)
#define I2C_SDA (5)
#define I2C_SCL (4)

typedef enum { DISPLAY_HOME, DISPLAY_SIGNAL, DISPLAY_PROGRESS } DisplayState;

class Display {
 public:
  Display();
  void redraw(uint32_t millis);
  void setState(DisplayState state);
  DisplayState getState();
  // The display can render signals normalized to values between 0..1
  void storeSignal(float normalizedSample);
  void setSig1(uint16_t sample);
  void setProgress(uint8_t percentage);
  void smoothProgressFinish(uint8_t frameDelay);

 private:
  SSD1306 *display;
  DisplayState dispState;
  uint16_t sig1;
  float signalBuffer[DISPLAY_WIDTH];
  uint16_t signalBufferOffset = 0;
  String serviceMessage;
  uint8_t progress;
};

}  // namespace espers

#endif


#include "display.h"

namespace espers {
Display::Display(ApplicationState* pState) {
  state = pState;
  // Initialize the OLED display using Wire library
  display = new SSD1306(I2C_ADDR, I2C_SDA, I2C_SCL);
  display->init();

  // TODO: figure out how to do this withouth reboot.
  if (state->disp_flipSreen) display->flipScreenVertically();
}
void Display::storeSignal(float normalizedSample) {
  //#define storeSignal(v) signalBuffer[signalBufferOffset++%DISPLAY_WIDTH]=(v)
  signalBuffer[signalBufferOffset++ % DISPLAY_WIDTH] = normalizedSample;
}

void Display::setProgress(uint8_t percentage) {
  this->progress = percentage % 100;
}
void Display::animateProgress(uint8_t percentage, uint8_t frameDelay) {
  if (percentage - progress < 0) {
    while (progress > percentage) {
      progress--;
      redraw(millis());
      delay(frameDelay);
    }
  } else {
    while (progress < percentage) {
      progress++;
      redraw(millis());
      delay(frameDelay);
    }
  }
}
void Display::redraw(uint32_t millis) {
  display->setContrast(state->disp_contrast);
  if (state->disp_invertColors) {  // Invert colors
    display->setColor(WHITE);
    display->fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    display->setColor(BLACK);
  } else {
    display->clear();
    display->setColor(WHITE);
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  switch (state->disp_state) {
    case DISPLAY_HOME:
      display->drawString(0, 0, "HOME");
      break;
    case DISPLAY_SIGNAL:
      // Draw midline.
      for (int i = 0; i < DISPLAY_WIDTH; i++)
        display->setPixel(i, DISPLAY_HEIGHT / 2);

      if (0) {  // Visualize signal as scattered pixels
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
          display->setPixel(
              i,
              DISPLAY_HEIGHT -
                  (DISPLAY_HEIGHT / 1.0f) *
                      signalBuffer[(signalBufferOffset + i) % DISPLAY_WIDTH]);
        }
      } else {  // Visualize signal as connected lines
        for (size_t i = 1; i < DISPLAY_WIDTH; i++) {
          float y0 = signalBuffer[(signalBufferOffset + i - 1) % DISPLAY_WIDTH];
          float y1 = signalBuffer[(signalBufferOffset + i) % DISPLAY_WIDTH];
          display->drawLine(i - 1,
                            DISPLAY_HEIGHT - (DISPLAY_HEIGHT / 1.0f) * y0, i,
                            DISPLAY_HEIGHT - (DISPLAY_HEIGHT / 1.0f) * y1);
        }
      }
      // Display raw value in top left
      display->drawString(0, 0,
                          String("SIGNAL " + String(state->disp_sig1, DEC)));
      // Display blemessage

      display->drawString(0, 12, state->disp_message1);
      // Temporary display for heartrate.
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(DISPLAY_WIDTH, 0, String(state->disp_heartrate));
      break;
    case DISPLAY_PROGRESS:
      // draw the progress bar
      display->drawProgressBar(0, 32, 120, 10, progress);

      // draw the percentage as String
      display->setTextAlignment(TEXT_ALIGN_CENTER);
      display->drawString(64, 15, String(progress) + "%");
      break;
  }
  if (0) {
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(DISPLAY_WIDTH, DISPLAY_HEIGHT - 10, String(millis));
  }
  display->display();
}

}  // namespace espers

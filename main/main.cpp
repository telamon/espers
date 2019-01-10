/*
 * main.cpp
 * Espers - A prototype wearable
 */

#include <Arduino.h>
#include <esp_log.h>
#include "settings.h"
#include "application_state.h"
#include "ble_comms.h"
#include "chrono.h"
#include "display.h"

#ifdef ENABLE_HEARTSENSOR
#include "heart.h"
#include <driver/adc.h>
#endif

#ifdef ENABLE_KINETICS
#include "kinetics.h"
#endif

#ifdef ENABLE_I2C
#include "i2c_hub.h"
#endif

static const char* LOG_TAG = "Espers";

// END OF SETTINGS
espers::ApplicationState state;
espers::Display* pDisplay;

#ifdef ENABLE_HEARTSENSOR
espers::Heart* pHeart;
#endif

espers::BLEComms* pBLEComms;
espers::Chrono* pChrono;

#ifdef ENABLE_ACCELLEROMETER

#endif


/* --------- Setup & Loop --- */
void setup() {
  ESP_LOGD(LOG_TAG, ">> Starting up");

  ESP_LOGD(LOG_TAG, ">> Initializing display");
  pDisplay = new espers::Display(&state);
  state.disp_state = espers::DisplayState::DISPLAY_PROGRESS;
  pDisplay->setProgress(0);
  pDisplay->redraw();

  for (int i = 0; i < DISPLAY_WIDTH; i++) {
    pDisplay->storeSignal(0.7);
  }
  pChrono = new espers::Chrono(&state);

#ifdef ENABLE_HEARTSENSOR
  ESP_LOGD(LOG_TAG, ">> Initializing heartrate processing");
  //pinMode(HEARTPIN, INPUT);
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(HEARTPIN, ADC_ATTEN_11db);
  pHeart = new espers::Heart(&state);
#endif

  pDisplay->animateProgress(30, 10);
  ESP_LOGD(LOG_TAG, ">> Initializing BLECommunications");
  pBLEComms = new espers::BLEComms(&state);

  pDisplay->animateProgress(100, 5);
  state.disp_state = espers::DisplayState::DISPLAY_SIGNAL;
}

void loop() {
  // Update application_state with fresh time.
  pChrono->updateTime();

#ifdef ENABLE_HEARTSENSOR
  // Store signal
  int sample = adc1_get_voltage(HEARTPIN);
  // int sample = analogRead(HEARTPIN);
  pDisplay->storeSignal(sample / 4095.f);
  state.disp_sig1 = sample;
  // Update signal processing
  pHeart->process(sample);
#endif

  // Redraw the screen
  pDisplay->redraw();

  delay(state.glob_delay);
}

/*
 * main.cpp
 * Espers - A prototype wearable
 */

#include <Arduino.h>
#include <driver/adc.h>
#include <esp_log.h>
#include "application_state.h"
#include "ble_comms.h"
#include "display.h"
#include "heart.h"
static const char* LOG_TAG = "Espers";

//#define HEARTPIN ADC1_CHANNEL_18
#define HEARTPIN 15

espers::ApplicationState state;
espers::Display* pDisplay;
espers::Heart* pHeart;
espers::BLEComms* pBLEComms;

/* --------- Setup & Loop --- */
void setup() {
  ESP_LOGD(LOG_TAG, ">> Starting up");

  ESP_LOGD(LOG_TAG, ">> Initializing display");
  pDisplay = new espers::Display(&state);
  state.disp_state = espers::DisplayState::DISPLAY_PROGRESS;
  pDisplay->setProgress(0);
  pDisplay->redraw(millis());

  for (int i = 0; i < DISPLAY_WIDTH; i++) {
    pDisplay->storeSignal(0.7);
  }

  ESP_LOGD(LOG_TAG, ">> Initializing heartrate processing");
  pinMode(HEARTPIN, INPUT);
  // adc1_config_width(ADC_WIDTH_12Bit);
  // adc1_config_channel_atten(HEARTPIN, ADC_ATTEN_11db);
  pHeart = new espers::Heart(&state);

  pDisplay->animateProgress(30, 10);
  ESP_LOGD(LOG_TAG, ">> Initializing BLECommunications");
  pBLEComms = new espers::BLEComms(&state);

  pDisplay->animateProgress(100, 5);
  state.disp_state = espers::DisplayState::DISPLAY_SIGNAL;
}

void loop() {
  uint32_t time = millis();
  // Store signal
  // int sample = adc1_get_voltage(HEARPIN);
  int sample = analogRead(HEARTPIN);
  pDisplay->storeSignal(sample / 4095.f);
  state.disp_sig1 = sample;

  pHeart->process(sample, time);
  pDisplay->redraw(time);

  delay(state.glob_delay);
}

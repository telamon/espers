/*
 * main.cpp
 * Espers - A prototype wearable
 */

#include <Arduino.h>
#include <esp_log.h>
#include "ble_comms.h"
#include "display.h"
#include "heart.h"

static const char* LOG_TAG = "Espers";

#define HEARTPIN 0

uint8_t globDelay = 20;
espers::Display* pDisplay;
espers::Heart* pHeart;
espers::BLEComms* pBLEComms;

/* --------- Setup & Loop --- */
void setup() {
  ESP_LOGD(LOG_TAG, ">> Starting up");

  ESP_LOGD(LOG_TAG, ">> Initializing display");
  pDisplay = new espers::Display();
  pDisplay->setState(espers::DisplayState::DISPLAY_PROGRESS);
  pDisplay->setProgress(10);
  pDisplay->redraw(millis());

  for (int i = 0; i < DISPLAY_WIDTH; i++) {
    pDisplay->storeSignal(0.7);
  }

  pDisplay->setProgress(20);
  pDisplay->redraw(millis());
  ESP_LOGD(LOG_TAG, ">> Initializing heartrate processing");
  pinMode(HEARTPIN, INPUT);
  pHeart = new espers::Heart();

  pDisplay->setProgress(30);
  pDisplay->redraw(millis());
  ESP_LOGD(LOG_TAG, ">> Initializing BLECommunications");
  pBLEComms = new espers::BLEComms();
  pHeart->setNotifyCharacteristic(pBLEComms->getHeartCharacteristic());

  pDisplay->smoothProgressFinish(5);
  pDisplay->setState(espers::DisplayState::DISPLAY_SIGNAL);
}

void loop() {
  uint32_t time = millis();
  // Store signal
  int sample = analogRead(HEARTPIN);
  pDisplay->storeSignal(sample / 4095.f);
  pDisplay->setSig1(sample);

  pHeart->process(sample, time);
  pDisplay->redraw(time);

  delay(globDelay);
}

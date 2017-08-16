#include "heart.h"
namespace espers {
Heart::Heart(ApplicationState* pState) {
  this->pState = pState;
  heartrate = 0;
  period = 0;
  nPulses = 0;
  released = true;
  prevHeart = 0;
  threshold = 200;
  pNotifyCharacteristic = NULL;
}

void Heart::setNotifyCharacteristic(BLECharacteristic* pCharacteristic) {
  pNotifyCharacteristic = pCharacteristic;
}

void Heart::process(uint16_t signal, uint32_t millis) {
  if (signal > (ADCMAX >> 1) + threshold) {
    if (released) {
      nPulses++;
      released = false;
    }
    // Dynamically tweak the threshold
    threshold = ((signal - (ADCMAX >> 1)) * 0.8 + threshold) / 2;
  } else {
    released = true;
  }
  if (period + PERIOD_INTERVAL < millis) {
    if (nPulses == 0) {
      // If no pulses were registered, lower the threshold but not past half
      // ADCMAX. (it's a sensor thing)
      threshold = std::max(ADCMAX >> 1, threshold - THRESHOLD_DECAY);
    }
    // Reset period state
    prevHeart = heartrate;
    heartrate = (nPulses * 20 + prevHeart) / 2;
    period = millis;
    nPulses = 0;

    // Export current heartrate for display
    pState->disp_heartrate = heartrate;

    // Broadcast new heartrate
    if (pNotifyCharacteristic != NULL) {
      pNotifyCharacteristic->setValue(String(heartrate).c_str());
      pNotifyCharacteristic->notify();
    }
  }
}

}  // namespace espers

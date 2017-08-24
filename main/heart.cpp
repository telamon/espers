#include "heart.h"
namespace espers {
Heart::Heart(ApplicationState* pState) {
  this->pState = pState;
  period = 0;
  nPulses = 0;
  released = true;
  prevHeart = 0;
}

void Heart::process(uint16_t signal) {
  uint64_t millis = pState->glob_uptimeMillis;
  if (signal > (ADCMAX >> 1) + pState->heart_threshold) {
    if (released) {
      nPulses++;
      released = false;
    }
    // Dynamically tweak the threshold
    pState->heart_threshold =
        ((signal - (ADCMAX >> 1)) * 0.5 + pState->heart_threshold) / 2;
  } else {
    released = true;
  }
  if (period + PERIOD_INTERVAL < millis) {
    if (nPulses == 0) {
      // If no pulses were registered, lower the threshold but not past half
      // ADCMAX. (it's a sensor thing)
      pState->heart_threshold =
          std::max(50, pState->heart_threshold - THRESHOLD_DECAY);
    }
    // Reset period state
    prevHeart = pState->heart_rate;
    pState->heart_rate = (nPulses * 20 + prevHeart) / 2;
    period = millis;
    nPulses = 0;

    // Broadcast new heartrate
    if (pState->ble_heartCharacteristic != NULL) {
      pState->ble_heartCharacteristic->setValue(
          String(pState->heart_rate).c_str());
      pState->ble_heartCharacteristic->notify();
    }
  }
}

}  // namespace espers

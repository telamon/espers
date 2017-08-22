#include "chrono.h"
#include <Arduino.h>
namespace espers {

Chrono::Chrono(ApplicationState* appState) { this->pState = appState; }

void Chrono::setTime(uint64_t epochMillis) {
  pState->glob_millisAtTime = millis();
  pState->glob_time = epochMillis;
}
uint64_t Chrono::getTime() {
  return pState->glob_time + millis() - pState->glob_millisAtTime;
}
}  // namespace espers

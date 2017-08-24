#include "chrono.h"
#include <Arduino.h>
namespace espers {

Chrono::Chrono(ApplicationState* appState) { this->pState = appState; }

void Chrono::setTime(uint64_t epochMillis) {
  pState->glob_millisAtTime = millis();
  pState->glob_timeOffset = epochMillis;
}
void Chrono::updateTime() {
  pState->glob_uptimeMillis = millis();
  pState->glob_timeMillis = pState->glob_timeOffset +
                            pState->glob_uptimeMillis -
                            pState->glob_millisAtTime;
  pState->glob_time = (time_t)(pState->glob_timeMillis / 1000);
}

}  // namespace espers

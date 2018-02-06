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
void reduceDuration(durationstruct_t* pDurs, uint64_t milliseconds) {
  pDurs->days = (milliseconds / DURD);
  milliseconds = milliseconds - pDurs->days * DURD;
  pDurs->hours = (milliseconds / DURH);
  milliseconds = milliseconds - pDurs->hours * DURH;
  pDurs->minutes = (milliseconds / DURM);
  milliseconds = milliseconds - pDurs->minutes * DURM;
  pDurs->seconds = (milliseconds / DURS);
  milliseconds = milliseconds - pDurs->seconds * DURS;
}

int humanizeDuration(char* cstring, uint64_t milliseconds) {
  durationstruct_t dstruct{};
  reduceDuration(&dstruct, milliseconds);

  uint8_t wcount = 0;
  if (dstruct.days > 0) wcount += sprintf(cstring, " %2id", dstruct.days);
  if (dstruct.hours > 0)
    wcount += sprintf((char*)(cstring + wcount), " %2ih", dstruct.hours);
  if (dstruct.minutes > 0 && dstruct.days == 0)
    wcount += sprintf((char*)(cstring + wcount), " %2im", dstruct.minutes);
  if (dstruct.seconds > 0 && dstruct.hours == 0 && dstruct.days == 0)
    wcount += sprintf((char*)(cstring + wcount), " %2is", dstruct.seconds);
  return wcount;
}

}  // namespace espers

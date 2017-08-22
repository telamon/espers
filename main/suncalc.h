/********* Suncalc.js #a617534 port to C++ ********/
#include <Arduino.h>
#ifndef SUNCALC_H_
#define SUNCALC_H_
namespace suncalc {

// calculates sun position for a given date and latitude/longitude
typedef struct PositionResult {
  float azimuth;
  float altitude;
} PositionResult;

PositionResult getPosition(uint64_t date, float lat, float lng);

typedef struct TimesResult {
  uint64_t solarNoon = -1;
  uint64_t nadir = -1;
  uint64_t sunrise = -1;
  uint64_t sunset = -1;
  uint64_t sunriseEnd = -1;
  uint64_t sunsetStart = -1;
  uint64_t dawn = -1;
  uint64_t dusk = -1;
  uint64_t nauticalDawn = -1;
  uint64_t nauticalDusk = -1;
  uint64_t nightEnd = -1;
  uint64_t night = -1;
  uint64_t goldenHourEnd = -1;
  uint64_t goldenHour = -1;
} TimesResult;
TimesResult* getTimes(TimesResult* pResult, uint64_t date, float lat,
                      float lng);

typedef enum {
  SUNRISE,
  SUNSET,
  SUNRISE_END,
  SUNSET_START,
  DAWN,
  DUSK,
  NAUTICAL_DAWN,
  NAUTICAL_DUSK,
  NIGHT_END,
  NIGHT,
  GOLDEN_HOUR_END,
  GOLDEN_HOUR,
} EventName;

typedef struct TimeEvent {
  float deg;
  EventName morning;
  EventName evening;
} TimeEvent;
// sun times configuration (angle, morning name, evening name)
#define TIMESLENGTH 6
static TimeEvent times[] = {
    {-0.833, SUNRISE, SUNSET}, {-0.3, SUNRISE_END, SUNSET_START},
    {-6, DAWN, DUSK},          {-12, NAUTICAL_DAWN, NAUTICAL_DUSK},
    {-18, NIGHT_END, NIGHT},   {6, GOLDEN_HOUR_END, GOLDEN_HOUR}};
}  // namespace suncalc
#endif

/********* Suncalc.js #a617534 port to C++ ********/
#include "suncalc.h"
namespace suncalc {
// shortcuts for easier to read formulas
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
/* Theese are already provided by "arduino.h"
#define sin Math.sin
#define cos Math.cos
#define tan Math.tan
#define asin Math.asin
#define atan Math.atan2
#define acos Math.acos
*/
#define rad (PI / 180)

// sun calculations are based on http://aa.quae.nl/en/reken/zonpositie.html
// formulas

// date/time constants and conversions

#define dayMs (1000 * 60 * 60 * 24)
#define J1970 2440588
#define J2000 2451545

#define toJulian(date) ((date) / dayMs - 0.5 + J1970)
#define fromJulian(j) ((uint64_t)(((j) + 0.5 - J1970) * dayMs))
#define toDays(date) (toJulian(date) - J2000)

// general calculations for position

// obliquity of the Earth
#define e (rad * 23.4397)

#define rightAscension(l, b) (atan2(sin(l) * cos(e) - tan(b) * sin(e), cos(l)))
#define declination(l, b) (asin(sin(b) * cos(e) + cos(b) * sin(e) * sin(l)))
#define azimuth(H, phi, dec) \
  (atan2(sin(H), cos(H) * sin(phi) - tan(dec) * cos(phi)))

#define altitude(H, phi, dec) \
  (asin(sin(phi) * sin(dec) + cos(phi) * cos(dec) * cos(H)))
#define siderealTime(d, lw) (rad * (280.16 + 360.9856235 * d) - lw)

float astroRefraction(float h) {
  if (h < 0)  // the following formula works for positive altitudes only.
    h = 0;    // if h = -0.08901179 a div/0 would occur.

  // formula 16.4 of "Astronomical Algorithms" 2nd edition by Jean Meeus
  // (Willmann-Bell, Richmond) 1998.
  // 1.02 / tan(h + 10.26 / (h + 5.10)) h in degrees, result in arc minutes ->
  // converted to rad:
  return 0.0002967 / tan(h + 0.00312536 / (h + 0.08901179));
}

#define solarMeanAnomaly(d) (rad * (357.5291 + 0.98560028 * (d)))

float eclipticLongitude(float M) {
  float C = rad * (1.9148 * sin(M) + 0.02 * sin(2 * M) +
                   0.0003 * sin(3 * M));  // equation of center
  float P = rad * 102.9372;               // perihelion of the Earth

  return M + C + P + PI;
}
typedef struct sunCoordsRet {
  float dec;
  float ra;
} sunCoordsRet;

sunCoordsRet sunCoords(float d) {
  float M = solarMeanAnomaly(d);
  float L = eclipticLongitude(M);

  return {.dec = (float)declination(L, 0), .ra = (float)rightAscension(L, 0)};
}
// calculations for sun times

#define J0 0.0009

#define julianCycle(d, lw) (round((d)-J0 - (lw) / (2 * PI)))

#define approxTransit(Ht, lw, n) (J0 + ((Ht) + (lw)) / (2 * PI) + (n))
#define solarTransitJ(ds, M, L) \
  (J2000 + (ds) + 0.0053 * sin(M) - 0.0069 * sin(2 * (L)))

#define hourAngle(h, phi, d) \
  (acos((sin(h) - sin(phi) * sin(d)) / (cos(phi) * cos(d))))

// returns set time for the given sun altitude
#define getSetJ(h, lw, phi, dec, n, M, L) \
  (solarTransitJ(approxTransit(hourAngle(h, phi, dec), lw, n), M, L))

PositionResult getPosition(uint64_t date, float lat, float lng) {
  float lw = rad * -lng;
  float phi = rad * lat;
  float d = toDays(date);
  sunCoordsRet c = sunCoords(d);
  float H = siderealTime(d, lw) - c.ra;

  return {.azimuth = (float)azimuth(H, phi, c.dec),
          .altitude = (float)altitude(H, phi, c.dec)};
}

void setValueInTimesResult(TimesResult* pRes, EventName name, uint64_t value) {
  switch (name) {
    case SUNRISE:
      pRes->sunrise = value;
      break;
    case SUNSET:
      pRes->sunset = value;
      break;
    case SUNRISE_END:
      pRes->sunriseEnd = value;
      break;
    case SUNSET_START:
      pRes->sunsetStart = value;
      break;
    case DAWN:
      pRes->dawn = value;
      break;
    case DUSK:
      pRes->dusk = value;
      break;
    case NAUTICAL_DAWN:
      pRes->nauticalDawn = value;
      break;
    case NAUTICAL_DUSK:
      pRes->nauticalDusk = value;
      break;
    case NIGHT_END:
      pRes->nightEnd = value;
      break;
    case NIGHT:
      pRes->night = value;
      break;
    case GOLDEN_HOUR_END:
      pRes->goldenHourEnd = value;
      break;
    case GOLDEN_HOUR:
      pRes->goldenHour = value;
      break;
  }
}
// calculates sun times for a given date and latitude/longitude

TimesResult* getTimes(TimesResult* pResult, uint64_t date, float lat,
                      float lng) {
  float lw = rad * -lng;
  float phi = rad * lat;

  float d = toDays(date);
  float n = julianCycle(d, lw);
  float ds = approxTransit(0, lw, n);

  float M = solarMeanAnomaly(ds);
  float L = eclipticLongitude(M);
  float dec = declination(L, 0);
  float Jnoon = solarTransitJ(ds, M, L);
  int i, len;
  float Jset, Jrise;

  pResult->solarNoon = fromJulian(Jnoon);
  pResult->nadir = fromJulian(Jnoon - 0.5);

  for (i = 0, len = TIMESLENGTH; i < len; i += 1) {
    TimeEvent time = times[i];

    Jset = getSetJ(time.deg * rad, lw, phi, dec, n, M, L);
    Jrise = Jnoon - (Jset - Jnoon);
    setValueInTimesResult(pResult, time.morning, fromJulian(Jrise));
    setValueInTimesResult(pResult, time.evening, fromJulian(Jset));
  }

  return pResult;
};

// moon calculations, based on http://aa.quae.nl/en/reken/hemelpositie.html
// formulas
/*
function moonCoords(d) { // geocentric ecliptic coordinates of the moon

    var L = rad * (218.316 + 13.176396 * d), // ecliptic longitude
        M = rad * (134.963 + 13.064993 * d), // mean anomaly
        F = rad * (93.272 + 13.229350 * d),  // mean distance

        l  = L + rad * 6.289 * sin(M), // longitude
        b  = rad * 5.128 * sin(F),     // latitude
        dt = 385001 - 20905 * cos(M);  // distance to the moon in km

    return {
        ra: rightAscension(l, b),
        dec: declination(l, b),
        dist: dt
    };
}*/
}  // namespace suncalc

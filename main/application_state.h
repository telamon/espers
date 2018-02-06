/*
 * Serializable application state
 */
#ifndef APPLICATION_STATE_H_
#define APPLICATION_STATE_H_
#include <BLECharacteristic.h>
#include <time.h>
namespace espers {

typedef enum { DISPLAY_HOME, DISPLAY_SIGNAL, DISPLAY_PROGRESS } DisplayState;

typedef struct ApplicationState {
  // Global
  uint16_t glob_delay = 25;
  uint64_t glob_timeOffset = 1503699181979;
  time_t glob_time = 0;
  uint64_t glob_timeMillis = 0;
  uint64_t glob_millisAtTime = 0;
  uint32_t glob_uptimeMillis = 0;
  float glob_posLat = 57.716077f;
  float glob_posLng = 11.924055f;

  // Display
  uint8_t disp_contrast = 255;
  bool disp_flipSreen = false;
  bool disp_invertColors = false;
  const char* disp_message1 = "<unitialized>";
  uint16_t disp_sig1;
  DisplayState disp_state = DISPLAY_HOME;

  // Heartrate calculator
  uint8_t heart_rate = 0;
  uint16_t heart_threshold = 200;

  // BLE/Communications
  BLECharacteristic* ble_heartCharacteristic = NULL;

} ApplicationState;

}  // namespace espers

#endif

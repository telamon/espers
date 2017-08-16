/*
 * Serializable application state
 */
#ifndef APPLICATION_STATE_H_
#define APPLICATION_STATE_H_

#include <BLECharacteristic.h>
namespace espers {

typedef enum { DISPLAY_HOME, DISPLAY_SIGNAL, DISPLAY_PROGRESS } DisplayState;

typedef struct ApplicationState {
  // Global
  uint16_t glob_delay = 5;

  // Display
  uint8_t disp_contrast = 255;
  bool disp_flipSreen = false;
  bool disp_invertColors = false;
  const char* disp_message1 = "<unitialized>";
  uint8_t disp_heartrate = 0;
  uint16_t disp_sig1;
  DisplayState disp_state = DISPLAY_HOME;

  // BLE/Communications
  BLECharacteristic* ble_heartCharacteristic = NULL;

} ApplicationState;

}  // namespace espers

#endif

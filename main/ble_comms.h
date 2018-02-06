
#ifndef BLE_COMMS_H_
#define BLE_COMMS_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <esp_log.h>
#include "application_state.h"
namespace espers {

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "0d531e57-d84e-4b12-830e-54acbefc22b1"
#define HEARTRATE_CHARACTERISTIC_UUID "cf4e7ce6-27b9-40df-932a-d06d6c0d44b3"
#define MSG_CHARACTERISTIC_UUID       "00faabe6-3d44-4980-a79e-92ffcf89610c"
#define CTRL_CHARACTERISTIC_UUID      "c385b478-3023-42af-9375-8c49ad0a8db1"

class BLEComms {
 public:
  BLEComms(ApplicationState* pState);

 private:
  BLEDevice ble;
  ApplicationState* pState;
  BLECharacteristic* pCtrlChara;
  const char* LOG_TAG = "Espers::BLEComms";
};

}  // namespace espers
#endif

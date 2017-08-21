
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
#define HEARTRATE_CHARACTERISTIC_UUID "8db3c4cc-8279-457d-9514-11127f8ae75a"
#define MSG_CHARACTERISTIC_UUID "8db3c5cc-8279-457d-9514-11127f8ae75a"
#define CTRL_CHARACTERISTIC_UUID "63b9c4cc-8279-458d-9514-11127f8ae75a"

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

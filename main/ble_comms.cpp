/*
 * ble_comms.cc
 * BLE Communications management.
 */
#include "ble_comms.h"
namespace espers {

class TestCb : public BLECharacteristicCallbacks {
 public:
  TestCb(ApplicationState* pState) : BLECharacteristicCallbacks() {
    this->pState = pState;
  }
  void onRead(BLECharacteristic* pCharacteristic) {
    pCharacteristic->setValue(pState->disp_message1);
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
    pState->disp_message1 = pCharacteristic->getValue().c_str();
  }

 private:
  ApplicationState* pState;
};

class CtrlCallbacks : public BLECharacteristicCallbacks {
 public:
  CtrlCallbacks(ApplicationState* pState) : BLECharacteristicCallbacks() {
    this->pState = pState;
  }
  void onRead(BLECharacteristic* pCharacteristic) {}
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string cmd = pCharacteristic->getValue();
  }

 private:
  ApplicationState* pState;
};

BLEComms::BLEComms(ApplicationState* pState) {
  this->pState = pState;
  ESP_LOGD(LOG_TAG, ">> BLE initialization starting.");
  // Initalize BLE env
  ble.init("Espers");

  // Create BLE Server
  BLEServer* pServer = new BLEServer();

  // Create the service
  BLEService* pService = pServer->createService(SERVICE_UUID);
  // Create the characteristics

  // heartrate characteristic
  // exposed on appstate
  pState->ble_heartCharacteristic = pService->createCharacteristic(
      HEARTRATE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  pState->ble_heartCharacteristic->setValue("<initializing>");

  // Message/display characteristic
  BLECharacteristic* pMsgChr = pService->createCharacteristic(
      MSG_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pMsgChr->setCallbacks(new TestCb(pState));

  pCtrlChara = pService->createCharacteristic(
      CTRL_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCtrlChara->setCallbacks(new CtrlCallbacks(pState));

  // Start the service (now it's usable but not advertising it's existance)
  pService->start();

  // Start advertising this server.
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  ESP_LOGD(LOG_TAG, ">> BLE initialization finished.");
}

}  // namespace espers

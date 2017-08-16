/*
 * ble_comms.cc
 * BLE Communications management.
 */
#include "ble_comms.h"
namespace espers {
/* --------- Initialize BLE --- */
/*class TestCb : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic){
  pCharacteristic->setValue(serviceMessage);
  }
  void onWrite(BLECharacteristic *pCharacteristic){
  serviceMessage = pCharacteristic->getValue();
  }
  };

  class CtrlCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic){
  }
  void onWrite(BLECharacteristic *pCharacteristic){
  std::string cmd = pCharacteristic->getValue();
  }
  };*/
BLECharacteristic* BLEComms::getHeartCharacteristic() { return pHeartChara; }
BLEComms::BLEComms() {
  ESP_LOGD(LOG_TAG, ">> BLE initialization starting.");
  // Initalize BLE env
  BLE::initServer("Espers");

  // Create BLE Server
  BLEServer* pServer = new BLEServer();

  // Create the service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create the characteristics
  pHeartChara = pService->createCharacteristic(
      HEARTRATE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  pHeartChara->setValue("<initializing>");
  // Message/display characteristic
  BLECharacteristic* pMsgChr = pService->createCharacteristic(
      MSG_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  // pMsgChr->setCallbacks(new TestCb());

  pCtrlChara = pService->createCharacteristic(
      CTRL_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  // pCtrlChara->setCallbacks(new CtrlCallbacks());

  // Start the service (now it's usable but not advertising it's existance)
  pService->start();

  // Start advertising this server.
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  ESP_LOGD(LOG_TAG, ">> BLE initialization finished.");
}
}  // namespace espers

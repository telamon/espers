/*
 * Heartrate signal processor
 */
#ifndef HEART_H_
#define HEART_H_

#include <Arduino.h>
#include <BLECharacteristic.h>
namespace espers {

#define ADCMAX (1 << 12)
#define THRESHOLD_DECAY 100
#define PERIOD_INTERVAL 3000
class Heart {
 public:
  Heart();
  void process(uint16_t signal, uint32_t millis);
  void setNotifyCharacteristic(BLECharacteristic* pCharacteristic);

 private:
  BLECharacteristic* pNotifyCharacteristic;
  int heartrate = 0;
  int period = 0;
  int nPulses = 0;
  int prevHeart = 0;
  int threshold = 200;
  bool released = true;
};

}  // namespace espers
#endif  // HEART_H_

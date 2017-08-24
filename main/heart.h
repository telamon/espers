/*
 * Heartrate signal processor
 */
#ifndef HEART_H_
#define HEART_H_

#include <Arduino.h>
#include "application_state.h"
namespace espers {

#define ADCMAX (1 << 12)
#define THRESHOLD_DECAY 500
#define PERIOD_INTERVAL 3000

class Heart {
 public:
  Heart(ApplicationState* pState);
  void process(uint16_t signal);

 private:
  ApplicationState* pState;
  int heartrate = 0;
  int period = 0;
  int nPulses = 0;
  int prevHeart = 0;
  bool released = true;
};

}  // namespace espers
#endif  // HEART_H_

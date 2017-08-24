#ifndef CHRONO_H_
#define CHRONO_H_
#include "application_state.h"
namespace espers {

class Chrono {
 public:
  Chrono(ApplicationState* appState);
  void setTime(uint64_t epochMillis);
  void updateTime();

 private:
  ApplicationState* pState;
};

}  // namespace espers
#endif

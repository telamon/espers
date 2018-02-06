
#include <chrono>
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
#define DURS 1000
#define DURM (60 * DURS)
#define DURH (60 * DURM)
#define DURD (24 * DURH)
#define MAKE_DUR(d, h, m, s) ((d)*DURD + (h)*DURH + (m)*DURM + (s)*DURS)
typedef struct durationstruct_t {
  uint16_t days;
  uint8_t hours;
  uint8_t minutes;
  uint16_t seconds;
} durationstruct_t;
void reduceDuration(durationstruct_t* pDurs, uint64_t milliseconds);
int humanizeDuration(char* cstring, uint64_t milliseconds);

}  // namespace espers
#endif

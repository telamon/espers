#ifndef KINETICS_H
#include "application_state.h"
namespace espers {
// Wait wat.. this is the temperature sensor, it's not supposed to be part
// of the kinetics component.
#ifdef HAVE_BME280
#define BME280_ADDR 0x60
#endif

class Kinetics {
  public:
    Kinetics(ApplicationState* pState);
  private:
    ApplicationState* pState;
    uint32_t steps;

};

} // namespace espers
#endif // KINETICS_H

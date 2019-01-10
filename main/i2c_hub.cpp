#ifdef ENABLE_I2C
#include "i2c_hub.h"
namespace espers {
  I2CHub::I2CHub() {
    this->port = portIter++;  
  }
} // namespace espers
#endif // ENABLE_I2C

#ifndef I2C_HUB_H
#define I2C_HUB_H

#include "application_state.h"
#include "driver/i2c.h"
namespace espers {

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */

static uint8_t portIter = 0; 
class I2CHub {
  public:
  I2CHub();
  esp_err_t read(uint8_t* data, size_t readLength);
  esp_err_t write(uint8_t* data, size_t writeLength);

  private:
  // ApplicationState* pState;
  uint8_t port;
};

} // namespace espers

#endif // I2C_HUB_H

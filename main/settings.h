#ifndef SETTINGS_H_
#define SETTINGS_H_

// SETTINGS
#define ESPERS_DEBUG 1
#define LANGUAGE "en"

// Enable heartrate calculation
 #define ENABLE_HEARTSENSOR

#ifdef ENABLE_HEARTSENSOR

// HEARTPIN is the pin you connect your sensor too
#define HEARTPIN ADC1_CHANNEL_0 // HWPin: 36
#endif

// Enable 9axis accellerometer 
#define ENABLE_KINETICS

#ifdef ENABLE_KINETICS
#define ENABLE_I2C
#endif

#ifdef ENABLE_I2C
// Configure the two i2c pins to which all the external preheprials are connected
#define I2C_SCL_PIN 52
#define I2C_SDA_PIN 51
#endif

#endif

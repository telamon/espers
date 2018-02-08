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
#define ENABLE_ACCELLEROMETER

#ifdef ENABLE_ACCELLEROMETER
#define ENABLE_I2C
#endif

#endif

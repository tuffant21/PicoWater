#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define SOLENOID_PIN 16
#define SOLENOID_RUNTIME_MS 300000 // five minutes

// time helper definintions
#define DATETIME_IGNORE -1
#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

// RTC has to be set because there is no battery to keep a date
#define TODAYS_YEAR 2024        // < 0..4095
#define TODAYS_MONTH 6          // < 1..12, 1 is January
#define TODAYS_DAY 21           // < 1..28,29,30,31 depending on month
#define TODAYS_DOTW FRIDAY      // < 0..6, 0 is Sunday, so 5 is Friday
#define TODAYS_HOUR 9 + 12      // < 0..23
#define TODAYS_MIN 50           // < 0..59
#define TODAYS_SEC 0            // < 0..59

// Logger Definitions
#define DEBUG 0
#define INFO 1
#define WARN 1
#define ERROR 1

// GPIO Helpers
#define GPIO_ON 1
#define GPIO_OFF 0

// Buffer Max Lengths
#define USB_COMMS_MAX_BUFFER_LENGTH 256
#define DATETIME_JSON_MAX_BUFFER_LENGTH 72
#define SSI_RESPONSE_MAX_BUFFER_LENGTH 1000

// The following definitions should be defined through the cmake build process
// #define PICO_W           1  // -DPICO_BOARD=""
// #define WIFI_SSID           // -DWIFI_SSID=""
// #define WIFI_PASSWORD       // -DWIFI_PASSWORD=""

#endif // DEFINITIONS_H
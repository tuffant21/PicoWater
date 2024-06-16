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
#define TODAYS_MONTH 5          // < 1..12, 1 is January
#define TODAYS_DAY 25           // < 1..28,29,30,31 depending on month
#define TODAYS_DOTW SATURDAY    // < 0..6, 0 is Sunday, so 5 is Friday
#define TODAYS_HOUR 1           // < 0..23
#define TODAYS_MIN 30            // < 0..59
#define TODAYS_SEC 0            // < 0..59

datetime_t ALARMS[] = {
    {
        .year  = DATETIME_IGNORE,
        .month = DATETIME_IGNORE,
        .day   = DATETIME_IGNORE,
        .dotw  = SUNDAY,
        .hour  = 6,
        .min   = 0,
        .sec   = 0
    },
    {
        .year  = DATETIME_IGNORE,
        .month = DATETIME_IGNORE,
        .day   = DATETIME_IGNORE,
        .dotw  = TUESDAY,
        .hour  = 6,
        .min   = 0,
        .sec   = 0
    },
    {
        .year  = DATETIME_IGNORE,
        .month = DATETIME_IGNORE,
        .day   = DATETIME_IGNORE,
        .dotw  = FRIDAY,
        .hour  = 6,
        .min   = 0,
        .sec   = 0
    }
};
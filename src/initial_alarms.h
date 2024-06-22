#ifndef INITIAL_ALARMS_H
#define INITIAL_ALARMS_H

#include "pico/stdlib.h"
#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

    datetime_t INITIAL_ALARMS[] = {
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

#ifdef __cplusplus
}
#endif

#endif // INITIAL_ALARMS_H
#ifndef ALARMS_H
#define ALARMS_H

#include <string.h> // memcpy
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/multicore.h"
#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        datetime_t *alarms;
        uint8_t length;
        uint8_t index;
    } alarms_t;

    bool alarms_init(const datetime_t *initialAlarms, const uint8_t initialAlarmsLength);
    bool alarms_equal(const datetime_t *a, const datetime_t *b);
    bool get_alarm_at_index(datetime_t *alarm, const int8_t index);
    bool next_alarm(datetime_t *nextAlarm);
    void get_alarms(alarms_t *alarms);
    bool add_alarm(const datetime_t *newAlarm);
    bool remove_alarm(const datetime_t *targetAlarm);

#ifdef __cplusplus
}
#endif

#endif // ALARMS_H
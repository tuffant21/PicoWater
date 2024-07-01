#include "alarms.h"

static alarms_t m_alarms = {
    .alarms = NULL,
    .index = 0,
    .length = 0
};

static void water_plants(void) {
    on_board_led_put(GPIO_ON);
    gpio_put(SOLENOID_PIN, GPIO_ON);
    sleep_ms(SOLENOID_RUNTIME_MS);
    gpio_put(SOLENOID_PIN, GPIO_OFF);
    on_board_led_put(GPIO_OFF);
}

static void water_plants_interrupt(void) {
    datetime_t nextAlarm;
    if (get_alarm_at_index(&nextAlarm, m_alarms.index)) {
        m_alarms.index++;

        if (m_alarms.index >= m_alarms.length) {
            m_alarms.index = 0;
        }

        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
    }

    multicore_reset_core1();
    multicore_launch_core1(water_plants);
}

void get_alarms(alarms_t *alarms) {
    *alarms = m_alarms;
}

bool get_alarm_at_index(datetime_t *alarm, const int8_t index) {
    // Check if the alarm list is empty
    if (m_alarms.length == 0) {
        return false;
    }

    // Calculate the actual index considering negative values
    int actual_index = index >= 0 ? index : m_alarms.length + index;

    // Check if the calculated index is within bounds
    if (actual_index < 0 || actual_index >= m_alarms.length) {
        return false;
    }

    *alarm = m_alarms.alarms[actual_index];
    return true;
}

bool add_alarm(const datetime_t *newAlarm) {
    // check for existing alarm before adding
    for (uint8_t i = 0; i < m_alarms.length; i++) {
        if (alarms_equal(&m_alarms.alarms[i], newAlarm)) {
            return false;
        }
    }

    m_alarms.alarms = realloc(m_alarms.alarms, sizeof(datetime_t) * (m_alarms.length + 1));
    if (m_alarms.alarms != NULL) {
        m_alarms.alarms[m_alarms.length] = *newAlarm;

        // if adding to end of array and index (next alarm) is zero, update index to end of array
        if (m_alarms.index == 0) {
            m_alarms.index = m_alarms.length;
        }

        m_alarms.length++;

        datetime_t nextAlarm;
        get_alarm_at_index(&nextAlarm, m_alarms.index - 1);
        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
        rtc_enable_alarm();

        return true;
    }

    return false;
}

bool remove_alarm(const datetime_t *targetAlarm) {
    bool alarmDeleted = false;

    for (uint8_t i = 0; i < m_alarms.length; i++) {
        if (alarms_equal(&m_alarms.alarms[i], targetAlarm)) {
            for (uint8_t j = i; j < m_alarms.length - 1; j++) {
                m_alarms.alarms[j] = m_alarms.alarms[j + 1];
            }

            m_alarms.length--;

            // if index (next alarm) is at end, decrement by one
            if (m_alarms.length > 0 && m_alarms.index == m_alarms.length) {
                m_alarms.index--;
            }

            m_alarms.alarms = realloc(m_alarms.alarms, sizeof(datetime_t) * m_alarms.length);
            if (m_alarms.alarms == NULL) {
                alarmDeleted = false;
                break;
            }

            alarmDeleted = true;
            break;
        }
    }

    if (alarmDeleted && m_alarms.length > 0) {
        datetime_t nextAlarm;
        get_alarm_at_index(&nextAlarm, m_alarms.index - 1);
        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
    } else {
        rtc_disable_alarm();
    }

    return alarmDeleted;
}

bool set_next_alarm(const uint8_t alarmId) {
    if (alarmId < 0 || alarmId >= m_alarms.length) {
        return false;
    }
    
    m_alarms.index = alarmId + 1;
    if (m_alarms.index >= m_alarms.length) {
        m_alarms.index = 0;
    }

    datetime_t nextAlarm;
    get_alarm_at_index(&nextAlarm, alarmId);
    rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
    return true;
}

bool alarms_equal(const datetime_t *a, const datetime_t *b) {
    return a->year == b->year &&
        a->month == b->month &&
        a->day == b->day &&
        a->dotw == b->dotw &&
        a->hour == b->hour &&
        a->min == b->min &&
        a->sec == b->sec;
}

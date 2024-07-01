#include "cgi.h"

static const char *cgi_handle_request_set_time(int index, int numParams, char *pcParam[], char *pcValue[]) {
datetime_t newTime = {
        .year = DATETIME_IGNORE,
        .month = DATETIME_IGNORE,
        .day = DATETIME_IGNORE,
        .dotw = DATETIME_IGNORE,
        .hour = DATETIME_IGNORE,
        .min = DATETIME_IGNORE,
        .sec = DATETIME_IGNORE
    };

    char datetimeStr[20] = {0};
    char dotwStr[2] = {0};

    // Parse parameters and set datetime values
    for (int i = 0; i < numParams; i++) {
        if (strncmp(pcParam[i], "datetime", strlen("datetime")) == 0) {
            strncpy(datetimeStr, pcValue[i], sizeof(datetimeStr) - 1);
        } else if (strncmp(pcParam[i], "dotw", strlen("dotw")) == 0) {
            strncpy(dotwStr, pcValue[i], sizeof(dotwStr) - 1);
        }
    }

    // Validate that both parameters are provided
    if (strlen(datetimeStr) == 0 || strlen(dotwStr) != 1 || !is_numeric(dotwStr)) {
        return "/index.shtml";
    }

    newTime.dotw = atoi(dotwStr);
    if (newTime.dotw < 0 || newTime.dotw > 6) {
        return "/index.shtml";
    }

    // URL decode the datetime string
    url_decode(datetimeStr);

    // Parse the datetime string
    char year[5] = {0}, month[3] = {0}, day[3] = {0}, hour[3] = {0}, min[3] = {0};
    if (sscanf(datetimeStr, "%4s-%2s-%2sT%2s:%2s", year, month, day, hour, min) != 5) {
        return "/index.shtml";
    }

    if (
        is_numeric(year) &&
        is_numeric(month) &&
        is_numeric(day) &&
        is_numeric(hour) &&
        is_numeric(min)
    ) {
        newTime.year = atoi(year);
        newTime.month = atoi(month);
        newTime.day = atoi(day);
        newTime.hour = atoi(hour);
        newTime.min = atoi(min);
        newTime.sec = 0;

        rtc_set_datetime(&newTime);
        busy_wait_us(64);
    }

    return "/index.shtml";
}

static const char *cgi_handle_request_add_alarm(int index, int numParams, char *pcParam[], char *pcValue[]) {
    datetime_t newAlarm = {
        .year = DATETIME_IGNORE,
        .month = DATETIME_IGNORE,
        .day = DATETIME_IGNORE,
        .dotw = DATETIME_IGNORE,
        .hour = DATETIME_IGNORE,
        .min = DATETIME_IGNORE,
        .sec = DATETIME_IGNORE
    };

    // Parse parameters and set datetime values
    for (int i = 0; i < numParams; i++) {
        if (strncmp(pcParam[i], "year", strlen("year")) == 0) {
            newAlarm.year = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "month", strlen("month")) == 0) {
            newAlarm.month = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "day", strlen("day")) == 0) {
            newAlarm.day = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "dotw", strlen("dotw")) == 0) {
            newAlarm.dotw = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "hour", strlen("hour")) == 0) {
            newAlarm.hour = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "min", strlen("min")) == 0) {
            newAlarm.min = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        } else if (strncmp(pcParam[i], "sec", strlen("sec")) == 0) {
            newAlarm.sec = strlen(pcValue[i]) == 0 ? DATETIME_IGNORE : atoi(pcValue[i]);
        }
    }

    // If year is set, month, day, dotw, hour, min, sec is required.
    // If month is set, day, dotw, hour, min, sec is required. 
    // If day is set, dotw, hour, min, sec is required.
    // If dotw is set, hour, min, sec is required.
    // If hour is set, min, sec is required.
    // Sec is always required. 

    // Check the required fields based on the logic
    if (newAlarm.year != DATETIME_IGNORE) {
        if (newAlarm.month == DATETIME_IGNORE || newAlarm.day == DATETIME_IGNORE ||
            newAlarm.dotw == DATETIME_IGNORE || newAlarm.hour == DATETIME_IGNORE ||
            newAlarm.min == DATETIME_IGNORE || newAlarm.sec == DATETIME_IGNORE) {
            return "/index.shtml";
        }
    } else if (newAlarm.month != DATETIME_IGNORE) {
        if (newAlarm.day == DATETIME_IGNORE || newAlarm.dotw == DATETIME_IGNORE ||
            newAlarm.hour == DATETIME_IGNORE || newAlarm.min == DATETIME_IGNORE ||
            newAlarm.sec == DATETIME_IGNORE) {
            return "/index.shtml";
        }
    } else if (newAlarm.day != DATETIME_IGNORE) {
        if (newAlarm.dotw == DATETIME_IGNORE || newAlarm.hour == DATETIME_IGNORE ||
            newAlarm.min == DATETIME_IGNORE || newAlarm.sec == DATETIME_IGNORE) {
            return "/index.shtml";
        }
    } else if (newAlarm.dotw != DATETIME_IGNORE) {
        if (newAlarm.hour == DATETIME_IGNORE || newAlarm.min == DATETIME_IGNORE ||
            newAlarm.sec == DATETIME_IGNORE) {
            return "/index.shtml";
        }
    } else if (newAlarm.hour != DATETIME_IGNORE) {
        if (newAlarm.min == DATETIME_IGNORE || newAlarm.sec == DATETIME_IGNORE) {
            return "/index.shtml";
        }
    } else if (newAlarm.sec == DATETIME_IGNORE) {
        // sec is always required
        return "/index.shtml";
    }

    // Add the new alarm
    add_alarm(&newAlarm);
    return "/index.shtml";
}

static const char *cgi_handle_request_remove_alarm(int index, int numParams, char *pcParam[], char *pcValue[]) {
    // Find the alarm-index parameter
    int alarmIndex = -1;
    for (int i = 0; i < numParams; i++) {
        if (strcmp(pcParam[i], "alarm-index") == 0) {
            // Convert the parameter value to an integer
            alarmIndex = atoi(pcValue[i]);
            break;
        }
    }

    // If alarm-index is not found or is negative, do nothing
    if (alarmIndex < 0) {
        return "/index.shtml";
    }

    // Check if alarm-index is within range
    alarms_t alarms;
    get_alarms(&alarms);
    if (alarmIndex >= alarms.length) {
        return "/index.shtml";
    }

    // Remove the alarm at the specified index
    datetime_t targetAlarm;
    if (get_alarm_at_index(&targetAlarm, alarmIndex)) {
        remove_alarm(&targetAlarm);
    }

    return "/index.shtml";
}

static const char *cgi_handle_request_change_alarm(int index, int numParams, char *pcParam[], char *pcValue[]) {
    for (uint8_t i = 0; i < numParams; i++) {
        if (
            strncmp("id", pcParam[i], strlen("id")) == 0 &&
            is_numeric(pcValue[i])
        ) {
            uint8_t alarmId = atoi(pcValue[i]);
            set_next_alarm(alarmId);
            break;
        }
    }

    return "/index.shtml";
}

static const char *cgi_handle_request_toggle_power(int index, int numParams, char *pcParam[], char *pcValue[]) {
    bool powered = sys_info_get_gpio_value();
    sys_info_set_gpio_value(!powered);
    return "/index.shtml";
}

static const tCGI cgi_handlers[] = {
    { "/set-time.cgi", cgi_handle_request_set_time },
    { "/add-alarm.cgi", cgi_handle_request_add_alarm },
    { "/remove-alarm.cgi", cgi_handle_request_remove_alarm },
    { "/change-alarm.cgi", cgi_handle_request_change_alarm },
    { "/toggle-power.cgi", cgi_handle_request_toggle_power },
};

bool cgi_init(void) {
    http_set_cgi_handlers(cgi_handlers, 5);
}

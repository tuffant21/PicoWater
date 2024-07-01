#include "helpers.h"

void log_datetime(const log_function_t log, const char *prefix, const datetime_t *time) {
    char year_str[5], month_str[3], day_str[3], dotw_str[2], hour_str[3], min_str[3], sec_str[3];
    
    snprintf(year_str, sizeof(year_str), (time->year == -1) ? "****" : "%04d", time->year);
    snprintf(month_str, sizeof(month_str), (time->month == -1) ? "**" : "%02d", time->month);
    snprintf(day_str, sizeof(day_str), (time->day == -1) ? "**" : "%02d", time->day);
    snprintf(dotw_str, sizeof(dotw_str), (time->dotw == -1) ? "*" : "%01d", time->dotw);
    snprintf(hour_str, sizeof(hour_str), (time->hour == -1) ? "**" : "%02d", time->hour);
    snprintf(min_str, sizeof(min_str), (time->min == -1) ? "**" : "%02d", time->min);
    snprintf(sec_str, sizeof(sec_str), "%02d", time->sec);

    log(
        "%s%s-%s-%s %s %s:%s:%s",
        prefix,
        year_str,
        month_str,
        day_str,
        dotw_str,
        hour_str,
        min_str,
        sec_str
    );
}

bool sscanf_datetime(datetime_t *time, const char* value, const bool allowIgnore) {
    char year[5] = "\0";
    char month[3] = "\0";
    char day[3] = "\0";
    char dotw[2] = "\0";
    char hour[3] = "\0";
    char min[3] = "\0";
    char sec[3] = "\0";

    int sscanfRes = sscanf(
        value,
        "%4s-%2s-%2s %1s %2s:%2s:%2s",
        year, month, day, dotw, hour, min, sec
    );

    if (sscanfRes == 7) {
        // Year
        if (strcmp(year, "****") == 0) {
            if (!allowIgnore) return false;
            time->year = -1;
        } else if (is_numeric(year)) {
            time->year = (int16_t) atoi(year);
        } else {
            return false;
        }

        // Month
        if (strcmp(month, "**") == 0) {
            if (!allowIgnore) return false;
            time->month = -1;
        } else if (is_numeric(month)) {
            time->month = (int8_t) atoi(month);
        } else {
            return false;
        }

        // Day
        if (strcmp(day, "**") == 0) {
            if (!allowIgnore) return false;
            time->day = -1;
        } else if (is_numeric(day)) {
            time->day = (int8_t) atoi(day);
        } else {
            return false;
        }

        // Day of the week
        if (strcmp(dotw, "*") == 0) {
            if (!allowIgnore) return false;
            time->dotw = -1;
        } else if (is_numeric(dotw)) {
            time->dotw = (int8_t) atoi(dotw);
        } else {
            return false;
        }

        // Hour
        if (strcmp(hour, "**") == 0) {
            if (!allowIgnore) return false;
            time->hour = -1;
        } else if (is_numeric(hour)) {
            time->hour = (int8_t) atoi(hour);
        } else {
            return false;
        }

        // Minute
        if (strcmp(min, "**") == 0) {
            if (!allowIgnore) return false;
            time->min = -1;
        } else if (is_numeric(min)) {
            time->min = (int8_t) atoi(min);
        } else {
            return false;
        }

        // Second
        if (strcmp(sec, "**") == 0) {
            if (!allowIgnore) return false;
            time->sec = -1;
        } else if (is_numeric(sec)) {
            time->sec = (int8_t) atoi(sec);
        } else {
            return false;
        }

        return true;
    }

    return false;
}

bool is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

int from_hex(char ch) {
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

void url_decode(char *str) {
    char *pstr = str, *buf = str;

    while (*pstr) {
        if (*pstr == '%') {
            if (pstr[1] && pstr[2]) {
                *buf++ = (char)(from_hex(pstr[1]) << 4 | from_hex(pstr[2]));
                pstr += 2;
            }
        } else if (*pstr == '+') {
            *buf++ = ' ';
        } else {
            *buf++ = *pstr;
        }
        pstr++;
    }

    *buf = '\0';
}

void on_board_led_init(void) {
#if !defined(PICO_W)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

void on_board_led_put(bool on) {
#if defined(PICO_W)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
#else
    gpio_put(PICO_DEFAULT_LED_PIN, on);
#endif
}
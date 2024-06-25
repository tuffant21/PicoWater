#include "usb_comms.h"

void handle_request_help(void) {
    log_info("     __  __________    ____ ");
    log_info("    / / / / ____/ /   / __ \\");
    log_info("   / /_/ / __/ / /   / /_/ /");
    log_info("  / __  / /___/ /___/ ____/");
    log_info(" /_/ /_/_____/_____/_/");
    log_info("");
    log_info("clear");
    log_info("    clears the screen");
    log_info("");
    log_info("get-time");
    log_info("    gets the RTC current value on the pico");
    log_info("");
    log_info("set-time=TIME");
    log_info("    sets the RTC value on the pico");
    log_info("");
    log_info("get-alarms");
    log_info("    prints all alarms currently in rotation, with > representing the next alarm to fire");
    log_info("");
    log_info("add-alarm=ALARM");
    log_info("    adds an alarm to the end of the alarms array");
    log_info("");
    log_info("remove-alarm=ALARM");
    log_info("    removes a matching alarm from the array or does nothing if input is not found");
    log_info("");
    log_info("set-gpio-value=BOOL");
    log_info("    sets the gpio value for the solenoid valve");
    log_info("");

#if defined(PICO_W)
    log_info("get-ip-address");
    log_info("    gets the router assigned ip address for this device");
    log_info("");
#endif

    log_info("The TIME argument should be in the format of YYYY-MM-DD d HH:MM:SS.");
    log_info("    (e.g., 2024-06-17 1 14:30:00 for June 17, 2024, Monday, 14:30:00).");
    log_info("");
    log_info("The ALARM argument should be in the format of YYYY-MM-DD d HH:MM:SS with *'s representing ignore.");
    log_info("    (e.g., ****-**-** 0 06:00:00 for any year, any month, any day, Sunday, 06:00:00).");
    log_info("");
    log_info(" The BOOL argument should either be a 1 or a 0. 1 representing on and 0 representing off");
}

void handle_request_clear_screen(void) {
    for(uint8_t i = 0; i < 80; i++) {
        echo("\n");
    }
}

bool is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

bool sscanf_datetime(datetime_t *time, const char* buffer, const bool allowIgnore) {
    char action[20] = "\0";
    char year[5] = "\0";
    char month[3] = "\0";
    char day[3] = "\0";
    char dotw[2] = "\0";
    char hour[3] = "\0";
    char min[3] = "\0";
    char sec[3] = "\0";

    int sscanfRes = sscanf(
        buffer,
        "%19[^=]=%4s-%2s-%2s %1s %2s:%2s:%2s",
        action, year, month, day, dotw, hour, min, sec
    );

    if (sscanfRes == 8) {
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

void print_datetime(const char *prefix, const datetime_t *time) {
    char year_str[5], month_str[3], day_str[3], dotw_str[2], hour_str[3], min_str[3], sec_str[3];
    
    snprintf(year_str, sizeof(year_str), (time->year == -1) ? "****" : "%04d", time->year);
    snprintf(month_str, sizeof(month_str), (time->month == -1) ? "**" : "%02d", time->month);
    snprintf(day_str, sizeof(day_str), (time->day == -1) ? "**" : "%02d", time->day);
    snprintf(dotw_str, sizeof(dotw_str), (time->dotw == -1) ? "*" : "%01d", time->dotw);
    snprintf(hour_str, sizeof(hour_str), (time->hour == -1) ? "**" : "%02d", time->hour);
    snprintf(min_str, sizeof(min_str), (time->min == -1) ? "**" : "%02d", time->min);
    snprintf(sec_str, sizeof(sec_str), "%02d", time->sec);

    log_info(
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

void handle_request_get_rtc_time(void) {
    datetime_t currentTime;
    rtc_get_datetime(&currentTime);
    print_datetime("RTC is now: ", &currentTime);
}

void handle_request_set_rtc_time(const char *buffer) {
    datetime_t setTime;
    
    if (sscanf_datetime(&setTime, buffer, false)) {
        rtc_set_datetime(&setTime);
        
        // Note that after setting the RTC date and time, a subsequent read of the values (e.g. via rtc_get_datetime())
        // may not reflect the new setting until up to three cycles of the potentially-much-slower RTC clock domain have
        // passed. This represents a period of 64 microseconds with the default RTC clock configuration.
        sleep_us(64);

        handle_request_get_rtc_time();
    } else {
        log_error("invalid datetime format");
    }
}

void handle_request_get_alarms(void) {
    alarms_t alarms;
    get_alarms(&alarms);

    datetime_t currentAlarm;
    if (alarms.length == 0 || !get_alarm_at_index(&currentAlarm, alarms.index - 1)) {
        log_info("There are 0 alarms.");
        return;
    }

    handle_request_get_rtc_time();
    log_info("");

    for (uint8_t i = 0; i < alarms.length; i++) {
        if (alarms_equal(&currentAlarm, &alarms.alarms[i])) {
            print_datetime("> ", &alarms.alarms[i]);
        } else {
            print_datetime("  ", &alarms.alarms[i]);
        }
    }
}

void handle_request_add_alarm(const char *buffer) {
    datetime_t time;

    if (sscanf_datetime(&time, buffer, true)) {
        alarms_t alarms;
        get_alarms(&alarms);

        for (uint8_t i = 0; i < alarms.length; i++) {
            if (alarms_equal(&alarms.alarms[i], &time)) {
                print_datetime("Alarm already exists: ", &time);
                return;
            }
        }

        add_alarm(&time);
        get_alarms(&alarms); // update alarms again after adding
        datetime_t nextAlarm;
        get_alarm_at_index(&nextAlarm, alarms.index - 1);
        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
        rtc_enable_alarm();
        print_datetime("Alarm added: ", &time);
    } else {
        log_error("invalid datetime format");
    }
}

void handle_request_remove_alarm(const char *buffer) {
    datetime_t time;

    if (sscanf_datetime(&time, buffer, true)) {
        if (remove_alarm(&time)) {
            alarms_t alarms;
            get_alarms(&alarms);

            if (alarms.length > 0) {
                datetime_t nextAlarm;
                get_alarm_at_index(&nextAlarm, alarms.index - 1);
                rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
            } else {
                rtc_disable_alarm();
            }

            print_datetime("Alarm removed: ", &time);
        } else {
            print_datetime("Alarm not found: ", &time);
        }
    } else {
        log_error("invalid datetime format");
    }
}

void handle_request_set_gpio_value(const char *buffer) {
    int value;
    int sscanfRes = sscanf(
        buffer,
        "set-gpio-value=%d",
        &value
    );

    if (sscanfRes == 1 && value == 0 || value == 1) {
        on_board_led_put(value);
        gpio_put(SOLENOID_PIN, value);
        log_info("setting gpio value to %d", value);
    } else {
        log_info("invalid value");
    }
}

#if defined(PICO_W)
void handle_request_get_ip_address(void) {
    struct netif *netif = netif_list;
    while (netif != NULL) {
        if (netif_is_up(netif)) {
            log_info("IP Address: %s", ip4addr_ntoa(netif_ip4_addr(netif)));
        }
        netif = netif->next;
    }
}
#endif

void handle_newline_or_carriage_return(const char *buffer) {
    // check for a help request
    if (
        strncmp(buffer, "help", strlen("help")) == 0
    ) {
        handle_request_help();
    }
    // clear screen
    else if (
        strncmp(buffer, "clear", strlen("clear")) == 0
    ) {
        handle_request_clear_screen();
    }
    // get current time
    else if (
        strncmp("get-time", buffer, strlen("get-time")) == 0
    ) {
        handle_request_get_rtc_time();
    }
    // check for a set time request
    else if (
        strncmp("set-time", buffer, strlen("set-time")) == 0 &&
        strlen(buffer) == strlen("set-time=YYYY-MM-DD d HH:MM:SS")
    ) {
        handle_request_set_rtc_time(buffer);
    }
    // get alarms
    else if (
        strncmp("get-alarms", buffer, strlen("get-alarms")) == 0
    ) {
        handle_request_get_alarms();
    }
    // add alarm
    else if (
        strncmp("add-alarm", buffer, strlen("add-alarm")) == 0 &&
        strlen(buffer) == strlen("add-alarm=YYYY-MM-DD d HH:MM:SS")
    ) {
        handle_request_add_alarm(buffer);
    }
    // remove alarm
    else if (
        strncmp("remove-alarm", buffer, strlen("remove-alarm")) == 0 &&
        strlen(buffer) == strlen("remove-alarm=YYYY-MM-DD d HH:MM:SS")
    ) {
        handle_request_remove_alarm(buffer);
    }
    // set gpio value
    else if (
        strncmp("set-gpio-value", buffer, strlen("set-gpio-value")) == 0 &&
        strlen(buffer) == strlen("set-gpio-value=v")
    ) {
        handle_request_set_gpio_value(buffer);
    }

#if defined(PICO_W)
    else if (
        strncmp("get-ip-address", buffer, strlen("get-ip-address")) == 0
    ) {
        handle_request_get_ip_address();
    }
#endif

    // command not recognized
    else {
        log_warn("Command [%s] not recognized. Try typing \"help\" to see a list of commands.", buffer);
    }
}

void usb_comms_handle_input(void) {
    static uint8_t index = 0;
    static uint8_t hasShowInputPrompt = 0;
    static char inputBuffer[USB_COMMS_MAX_BUFFER_LENGTH];

    if (index == 0 && !hasShowInputPrompt) {
        echo("\n\n> ");
        hasShowInputPrompt = 1;
    }

    int c = getchar_timeout_us(1000);

    if (c != PICO_ERROR_TIMEOUT) {
        if (c == '\n' || c == '\r') {
            inputBuffer[index] = '\0';
            index = 0;
            hasShowInputPrompt = 0;
            handle_newline_or_carriage_return(inputBuffer);
        } else if (index >= USB_COMMS_MAX_BUFFER_LENGTH - 1) {
            index = 0;
            inputBuffer[index] = '\0';
            hasShowInputPrompt = 0;
            log_error("Input exceeded %d and did not recieve newline or carriage return. Resetting index", USB_COMMS_MAX_BUFFER_LENGTH);
        } else {
            // c == 8 is backspace
            // note 8 is backspace on a mac keyboard. This may be different for other keyboards
            if (c == 8) {
                if (index > 0) {
                    index--;
                    // echo backspace, print a space to clear the previous letter on screen, then echo backspace again to have the correct position
                    echo("%c %c", c, c);
                } // else do nothing
            } else {
                inputBuffer[index++] = c;
                echo("%c", c); // print the character to the screen
            }
        }
    }
}
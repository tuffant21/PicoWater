#include "usb_comms.h"

static void handle_request_help(void) {
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
    log_info("get-uptime");
    log_info("    gets how long the system has been up for");
    log_info("");
    log_info("get-temp");
    log_info("    gets the temperature of the onboard cpu");
    log_info("");

#if defined(PICO_W)
    log_info("get-ip");
    log_info("    gets the router assigned ip address for this device");
    log_info("");
    log_info("connect-to-network=CONNECT");
    log_info("    attempts to connect to the defined network");
    log_info("");
#endif

    log_info("The TIME argument should be in the format of YYYY-MM-DD d HH:MM:SS.");
    log_info("    (e.g., 2024-06-17 1 14:30:00 for June 17, 2024, Monday, 14:30:00).");
    log_info("");
    log_info("The ALARM argument should be in the format of YYYY-MM-DD d HH:MM:SS with *'s representing ignore.");
    log_info("    (e.g., ****-**-** 0 06:00:00 for any year, any month, any day, Sunday, 06:00:00).");
    log_info("");
    log_info("The BOOL argument should either be a 1 or a 0. 1 representing on and 0 representing off");
#if defined(PICO_W)
    log_info("");
    log_info("The CONNECT argument should be in the format of #SSID#PASSWORD#AUTH where the first # defines the");
    log_info("    delimiter you want and the following #'s representing the delimiter to split the string at. The");
    log_info("    following values represent the available auth values. 0=CYW43_AUTH_OPEN, 1=CYW43_AUTH_WPA_TKIP_PSK,");
    log_info("    2=CYW43_AUTH_WPA2_AES_PSK, 3=CYW43_AUTH_WPA2_MIXED_PSK");
    log_info("    (e.g. ,my_network,my_password,2 would delimit the string with a comma, the first deliminated string");
    log_info("    would be the network ssid, the second the password, the third auth and would be CYW43_AUTH_WPA2_AES_PSK)");
#endif
}

static void handle_request_clear_screen(void) {
    for(uint8_t i = 0; i < 80; i++) {
        echo("\n");
    }
}

static void handle_request_get_rtc_time(void) {
    datetime_t now = sys_info_get_time();
    log_datetime(log_info, "RTC is now: ", &now);
}

static void handle_request_set_rtc_time(const char *buffer) {
    datetime_t setTime;
    
    if (sscanf_datetime(&setTime, buffer + strlen("set-time="), false)) {
        sys_info_set_time(&setTime);
        
        // Note that after setting the RTC date and time, a subsequent read of the values (e.g. via rtc_get_datetime())
        // may not reflect the new setting until up to three cycles of the potentially-much-slower RTC clock domain have
        // passed. This represents a period of 64 microseconds with the default RTC clock configuration.
        sleep_us(64);

        handle_request_get_rtc_time();
    } else {
        log_error("invalid datetime format");
    }
}

static void handle_request_get_alarms(void) {
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
            log_datetime(log_info, "> ", &alarms.alarms[i]);
        } else {
            log_datetime(log_info, "  ", &alarms.alarms[i]);
        }
    }
}

static void handle_request_add_alarm(const char *buffer) {
    datetime_t time;

    if (sscanf_datetime(&time, buffer + strlen("add-alarm="), true)) {
        add_alarm(&time)
            ? log_datetime(log_info, "Alarm added: ", &time)
            : log_datetime(log_warn, "could not add alarm", &time);
    } else {
        log_error("invalid datetime format");
    }
}

static void handle_request_remove_alarm(const char *buffer) {
    datetime_t time;

    if (sscanf_datetime(&time, buffer + strlen("remove-alarm="), true)) {
        remove_alarm(&time)
            ? log_datetime(log_info, "Alarm removed: ", &time)
            : log_datetime(log_warn, "Alarm not found: ", &time);
    } else {
        log_error("invalid datetime format");
    }
}

static void handle_request_set_gpio_value(const char *buffer) {
    int value;
    int sscanfRes = sscanf(
        buffer,
        "set-gpio-value=%d",
        &value
    );

    if (sscanfRes == 1 && value == 0 || value == 1) {
        sys_info_set_gpio_value(value);
        log_info("setting gpio value to %d", value);
    } else {
        log_info("invalid value");
    }
}

static void handle_request_get_uptime(void) {
    uint32_t uptime = sys_info_get_uptime();
    log_info("The uptime is now: %lu ms", uptime);
}

static void handle_request_get_temp(void) {
    float temp = sys_info_get_temp();
    float tempF = (temp * 9/5) + 32;
    log_info("current temp is: %.2f °C / %.2f °F", temp, tempF);
}

#if defined(PICO_W)
static void handle_request_get_ip_address(void) {
    char *ipAddress = sys_info_get_ip_address();

    if (strncmp("0.0.0.0", ipAddress, strlen("0.0.0.0")) == 0) {
        log_warn("Not connected to the network!");
    } else {
        log_info("IP Address: %s", ipAddress);
    }
}

static void handle_request_connect_to_network(const char *input) {
    char connect[256] = "\0";
    strncpy(connect, input + strlen("connect-to-network="), sizeof(connect) - 1);

    char delimiter[2] = { connect[0], '\0' };
    
    // Copy the input string to a modifiable buffer
    char buffer[256] = "\0";
    strncpy(buffer, connect + 1, sizeof(buffer) - 1); // Skip the first delimiter character

    // Tokenize the string based on the delimiter
    char *ssid = strtok(buffer, delimiter);
    char *password = strtok(NULL, delimiter);
    char *auth_str = strtok(NULL, delimiter);

    if (ssid && password && auth_str) {
        uint32_t auth = atoi(auth_str);

        log_info("Connecting to network:");
        log_info("    SSID: %s", ssid);
        log_info("    Password: %s", password);

        switch (auth) {
            case 0:
                auth = CYW43_AUTH_OPEN;
                log_info("    Auth: CYW43_AUTH_OPEN");
                break;
            case 1:
                auth = CYW43_AUTH_WPA_TKIP_PSK;
                log_info("    Auth: CYW43_AUTH_WPA_TKIP_PSK");
                break;
            case 2:
                auth = CYW43_AUTH_WPA2_AES_PSK;
                log_info("    Auth: CYW43_AUTH_WPA2_AES_PSK");
                break;
            case 3:
                auth = CYW43_AUTH_WPA2_MIXED_PSK;
                log_info("    Auth: CYW43_AUTH_WPA2_MIXED_PSK");
                break;
            default:
                log_info("Unknown authentication type - aborting");
                return;
        }

        bool connected = sys_info_connect_to_network(ssid, password, auth);
        connected
            ? log_info("successfull connected to the network!")
            : log_warn("could not connect to the network!");
    } else {
        log_info("Invalid CONNECT argument format.");
    }
}
#endif

static void handle_newline_or_carriage_return(const char *buffer) {
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
    // get uptime
    else if (
        strncmp("get-uptime", buffer, strlen("get-uptime")) == 0
    ) {
        handle_request_get_uptime();
    }
    // get temperature
    else if (
        strncmp("get-temp", buffer, strlen("get-temp")) == 0
    ) {
        handle_request_get_temp();
    }

#if defined(PICO_W)
    // get ip
    else if (
        strncmp("get-ip", buffer, strlen("get-ip")) == 0
    ) {
        handle_request_get_ip_address();
    } 
    // connect to network
    else if (
        strncmp("connect-to-network", buffer, strlen("connect-to-network")) == 0
    ) {
        handle_request_connect_to_network(buffer);
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
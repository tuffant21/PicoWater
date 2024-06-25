#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "stdbool.h"
#include "definitions.h"
#include "on_board_led.h"
#include "core_entries.h"
#include "initial_alarms.h"
#include "alarms.h"
#include "usb_comms.h"

#if defined(PICO_W)
#include "pico/cyw43_arch.h"
#include "network_comms.h"
#endif

bool init() {
    if(!stdio_init_all()) {
        return false;
    }

#if defined(PICO_W)
    if (cyw43_arch_init()) {
        log_error("Wi-Fi init failed");
        return false;
    }

    network_comms_init();
#endif

    on_board_led_init();

    datetime_t now = {
        .year  = TODAYS_YEAR,
        .month = TODAYS_MONTH,
        .day   = TODAYS_DAY,
        .dotw  = TODAYS_DOTW,
        .hour  = TODAYS_HOUR,
        .min   = TODAYS_MIN,
        .sec   = TODAYS_SEC
    };

    // Start the RTC
    rtc_init();
    rtc_set_datetime(&now);
    sleep_us(64);

    // init solenoid pin
    gpio_init(SOLENOID_PIN);
    gpio_set_dir(SOLENOID_PIN, GPIO_OUT);

    uint8_t initialAlarmsLength = sizeof(INITIAL_ALARMS) / sizeof(INITIAL_ALARMS[0]);
    alarms_init(INITIAL_ALARMS, initialAlarmsLength);

    return true;
}

int main() {
    if (!init()) {
        return 1;
    }

    datetime_t nextAlarm;
    if (next_alarm(&nextAlarm)) {
        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
    }

    // water alarm will keep firing forever
    while (true) {
        if (stdio_usb_connected()) {
            usb_comms_handle_input();
        } else {
            tight_loop_contents();
        }
    }
}
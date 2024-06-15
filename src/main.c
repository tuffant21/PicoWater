#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "pico/multicore.h"
#include "definitions.h"

#if PICO_W
#include "pico/cyw43_arch.h"
#endif

void onboard_led_put(uint8_t on) {
#if PICO_W
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
#else
    gpio_put(PICO_DEFAULT_LED_PIN, on);
#endif
}

uint8_t init() {
    if(!stdio_init_all()) {
        return 1;
    }

#if PICO_W
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return 1;
    }
#endif

#if !PICO_W
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif

    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];

    // Start on Friday 5th of June 2020 15:45:00
    datetime_t t = {
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
    rtc_set_datetime(&t);
    sleep_us(64);

    // init solenoid pin
    gpio_init(SOLENOID_PIN);
    gpio_set_dir(SOLENOID_PIN, GPIO_OUT);

    return 0;
}

void core1_entry() {
    onboard_led_put(1);
    gpio_put(SOLENOID_PIN, 1);
    sleep_ms(SOLENOID_RUNTIME_MS);
    gpio_put(SOLENOID_PIN, 0);
    onboard_led_put(0);
}

static void alarm_callback(void) {
    static uint8_t alarmIndex = 0;
    alarmIndex++;

    uint8_t length = sizeof(ALARMS) / sizeof(ALARMS[0]);
    if (alarmIndex >= length) {
        alarmIndex = 0;
    }

    // set next alarm
    rtc_set_alarm(&ALARMS[alarmIndex], &alarm_callback);

    multicore_reset_core1();
    multicore_launch_core1(core1_entry);
    stdio_flush();
}

int main() {
    if (init()) {
        return 1;
    }

    rtc_set_alarm(&ALARMS[0], &alarm_callback);

    // water alarm will keep firing forever
    while(1) {
        tight_loop_contents();
    }
}
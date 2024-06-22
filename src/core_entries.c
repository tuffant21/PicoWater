#include "core_entries.h"

void water_plants(void) {
    on_board_led_put(GPIO_ON);
    gpio_put(SOLENOID_PIN, GPIO_ON);
    sleep_ms(SOLENOID_RUNTIME_MS);
    gpio_put(SOLENOID_PIN, GPIO_OFF);
    on_board_led_put(GPIO_OFF);
}

void water_plants_interrupt(void) {
    datetime_t nextAlarm;
    if (next_alarm(&nextAlarm)) {
        rtc_set_alarm(&nextAlarm, &water_plants_interrupt);
    }

    multicore_reset_core1();
    multicore_launch_core1(water_plants);
}
#ifndef SYS_INFO_H
#define SYS_INFO_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/rtc.h"
#include "stdbool.h"
#include "definitions.h"
#include "helpers.h"

#if defined(PICO_W)
#include "pico/cyw43_arch.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void sys_info_init();
    float sys_info_get_temp();
    bool sys_info_get_gpio_value();
    void sys_info_set_gpio_value(bool value);
    uint32_t sys_info_get_uptime();
    datetime_t sys_info_get_time();
    void sys_info_set_time(datetime_t *time);

#if defined(PICO_W)
    bool sys_info_connect_to_network(const char *ssid, const char *password, const uint32_t auth);
    char *sys_info_get_ip_address();
#endif

#ifdef __cplusplus
}
#endif

#endif
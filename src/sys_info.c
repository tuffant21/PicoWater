#include "sys_info.h"

void sys_info_init() {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
}

float sys_info_get_temp() {
    const float voltage = adc_read() * 3.3f / (1 << 12);
    return 27.0 - (voltage - 0.706f) / 0.001721f;
}

bool sys_info_get_gpio_value() {
    return gpio_get(SOLENOID_PIN);
}

void sys_info_set_gpio_value(bool value) {
    on_board_led_put(value);
    gpio_put(SOLENOID_PIN, value);
}

uint32_t sys_info_get_uptime() {
    return to_ms_since_boot(get_absolute_time());
}

datetime_t sys_info_get_time() {
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}

void sys_info_set_time(datetime_t *time) {
    rtc_set_datetime(time);
}

#if defined(PICO_W)
bool sys_info_connect_to_network(const char *ssid, const char *password, const uint32_t auth) {
    int connected = cyw43_arch_wifi_connect_timeout_ms(ssid, password, auth, 30000);
    return connected == 0;
}

char *sys_info_get_ip_address() {
    struct netif *netif = netif_list;
    while (netif != NULL) {
        if (netif_is_up(netif)) {
            return ip4addr_ntoa(netif_ip4_addr(netif));
        }
        netif = netif->next;
    }
}
#endif
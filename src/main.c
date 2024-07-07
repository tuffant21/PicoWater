#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "stdbool.h"
#include "definitions.h"
#include "helpers.h"
#include "initial_alarms.h"
#include "alarms.h"
#include "usb_comms.h"
#include "sys_info.h"

#if PICO_W == 1
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "ssi.h"
#include "cgi.h"
#endif

bool init() {
    if(!stdio_init_all()) {
        return false;
    }

#if PICO_W == 1
    if (cyw43_arch_init()) {
        log_error("Wi-Fi init failed");
        return false;
    }

    cyw43_arch_enable_sta_mode();
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

    // init alarms
    uint8_t initialAlarmsLength = sizeof(INITIAL_ALARMS) / sizeof(INITIAL_ALARMS[0]);
    for (uint8_t i = 0; i < initialAlarmsLength; i++) {
        add_alarm(&INITIAL_ALARMS[i]);
    }

    sys_info_init();

#if PICO_W == 1
#if defined(WIFI_SSID) && defined(WIFI_PASSWORD)
    log_debug("attempting to connect to network [%s]...", WIFI_SSID);
    bool connected = sys_info_connect_to_network(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    connected
        ? log_debug("successfull connected to the network!")
        : log_warn("could not connect to the network!");
#endif

    char *ipAddress = sys_info_get_ip_address();
    if (strncmp("0.0.0.0", ipAddress, strlen("0.0.0.0")) != 0) {
        log_info("IP Address: %s", ipAddress);
    }

    log_debug("initializing http server...");
    httpd_init();
    log_debug("http server initialized!");

    log_debug("initalizing ssi...");
    ssi_init();
    log_debug("ssi initialized!");

    log_debug("initalizing cgi...");
    cgi_init();
    log_debug("cgi initialized!");
#endif

    return true;
}

int main() {
    if (!init()) {
        return 1;
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
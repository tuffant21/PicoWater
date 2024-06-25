#include "network_comms.h"

bool network_comms_init() {
    cyw43_arch_enable_sta_mode();

#if defined(WIFI_SSID) && defined(WIFI_PASSWORD)
    attempt_to_connect_to_network(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
#endif

    struct netif *netif = netif_list;
    while (netif != NULL) {
        if (netif_is_up(netif)) {
            log_info("IP Address: %s", ip4addr_ntoa(netif_ip4_addr(netif)));
        }
        netif = netif->next;
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

    return true;
}

bool attempt_to_connect_to_network(const char *ssid, const char *password, const uint32_t auth) {
    log_debug("attempting to connect to network [%s]...", ssid);
    int connected = cyw43_arch_wifi_connect_timeout_ms(ssid, password, auth, 30000);

    connected == 0
        ? log_debug("successfull connected to the network!")
        : log_warn("could not connect to the network!");

    return connected == 0;
}
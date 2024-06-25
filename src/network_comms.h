#ifndef NETWORK_COMMS_H
#define NETWORK_COMMS_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "stdbool.h"
#include "logger.h"
#include "ssi.h"
#include "cgi.h"

#ifdef __cplusplus
extern "C" {
#endif

    bool network_comms_init();
    bool attempt_to_connect_to_network(const char *ssid, const char *password, const uint32_t auth);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_COMMS_H
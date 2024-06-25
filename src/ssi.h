#ifndef SSI_H
#define SSI_H

#include "pico/stdlib.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

    bool ssi_init();
    uint16_t ssi_handler(const uint8_t index, const char *pcInsert, const uint8_t insertLength);

#ifdef __cplusplus
}
#endif

#endif // SSI_H
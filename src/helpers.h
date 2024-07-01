#ifndef HELPERS_H
#define HELPERS_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "stdbool.h"
#include "logger.h"
#include "definitions.h"

#if defined(PICO_W)
#include "pico/cyw43_arch.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*log_function_t)(const char *format, ...);

    void log_datetime(const log_function_t log, const char *prefix, const datetime_t *time);
    bool sscanf_datetime(datetime_t *time, const char* buffer, const bool allowIgnore);
    bool is_numeric(const char *str);
    int from_hex(char ch);
    void url_decode(char *str);
    void on_board_led_init(void);
    void on_board_led_put(bool on);

#ifdef __cplusplus
}
#endif

#endif // HELPERS_H
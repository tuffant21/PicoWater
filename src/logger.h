#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "pico/stdlib.h"
#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

    void echo(const char *format, ...);
    void log_debug(const char *format, ...);
    void log_info(const char *format, ...);
    void log_warn(const char *format, ...);
    void log_error(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H
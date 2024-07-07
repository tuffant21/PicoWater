#if PICO_W == 1

#ifndef CGI_H
#define CGI_H

#include <string.h>
#include "pico/stdlib.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "stdbool.h"
#include "logger.h"
#include "helpers.h"
#include "sys_info.h"
#include "alarms.h"

#ifdef __cplusplus
extern "C" {
#endif

    bool cgi_init(void);

#ifdef __cplusplus
}
#endif

#endif // CGI_H

#endif // PICO_W
#ifndef CGI_H
#define CGI_H

#include "pico/stdlib.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

    bool cgi_init(void);
    const char *cgi_handle_request_set_time(int index, int numParams, char *pcParam[], char *pcValue[]);
    const char *cgi_handle_request_add_alarm(int index, int numParams, char *pcParam[], char *pcValue[]);
    const char *cgi_handle_request_remove_alarm(int index, int numParams, char *pcParam[], char *pcValue[]);
    const char *cgi_handle_request_set_gpio_value(int index, int numParams, char *pcParam[], char *pcValue[]);

#ifdef __cplusplus
}
#endif

#endif // CGI_H
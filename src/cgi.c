#include "cgi.h"

// these are basically post requests

static const tCGI cgi_handlers[] = {
    { "/set-time.cgi", cgi_handle_request_set_time }
};

bool cgi_init(void) {
    http_set_cgi_handlers(cgi_handlers, 1);
}

const char *cgi_handle_request_set_time(int index, int numParams, char *pcParam[], char *pcValue[]) {
    return "/index.shtml";
}

const char *cgi_handle_request_add_alarm(int index, int numParams, char *pcParam[], char *pcValue[]) {

}

const char *cgi_handle_request_remove_alarm(int index, int numParams, char *pcParam[], char *pcValue[]) {

}

const char *cgi_handle_request_set_gpio_value(int index, int numParams, char *pcParam[], char *pcValue[]) {

}

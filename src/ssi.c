#include "ssi.h"

// these are basically get requests

// set SSI Tags
const char *ssi_tags[] = {
    "get-time",
    "set-time",
    "get-alarms",
    "add-alarm",
    "remove-alarm",
    "set-gpio-value"
};

bool ssi_init() {
    return true;
}

uint16_t ssi_handler(const uint8_t index, const char *pcInsert, const uint8_t insertLength) {
    size_t printed;

    switch (index) {
    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        break;

    case 5:
        break;

    default:
        printed = 0;
    }

    return printed;
}
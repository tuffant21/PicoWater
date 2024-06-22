#ifndef USB_COMMS_H
#define USB_COMMS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "definitions.h"
#include "stdbool.h"
#include "logger.h"
#include "core_entries.h"
#include "on_board_led.h"
#include "alarms.h"

#ifdef __cplusplus
extern "C" {
#endif

    void usb_comms_handle_input(void);

#ifdef __cplusplus
}
#endif

#endif // USB_COMMS_H
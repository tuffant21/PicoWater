#ifndef USB_COMMS_H
#define USB_COMMS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "definitions.h"
#include "stdbool.h"
#include "logger.h"
#include "alarms.h"
#include "sys_info.h"
#include "helpers.h"

#if PICO_W == 1
#include "pico/cyw43_arch.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void usb_comms_handle_input(void);

#ifdef __cplusplus
}
#endif

#endif // USB_COMMS_H
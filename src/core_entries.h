#ifndef CORE_ENTRIES_H
#define CORE_ENTRIES_H

#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/multicore.h"
#include "definitions.h"
#include "on_board_led.h"
#include "alarms.h"

#ifdef __cplusplus
extern "C" {
#endif

    void water_plants(void);
    void water_plants_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif // CORE_ENTRIES_H
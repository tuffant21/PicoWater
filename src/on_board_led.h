#ifndef ON_BOARD_LED_H
#define ON_BOARD_LED_H

#include "pico/stdlib.h"
#include "stdbool.h"
#include "definitions.h"

#if defined(PICO_W)
#include "pico/cyw43_arch.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void on_board_led_init(void);
    void on_board_led_put(bool on);

#ifdef __cplusplus
}
#endif

#endif // ON_BOARD_LED_H
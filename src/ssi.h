#if PICO_W == 1

#ifndef SSI_H
#define SSI_H

#include "pico/stdlib.h"
#include "stdbool.h"
#include "logger.h"
#include "sys_info.h"
#include "alarms.h"

#if PICO_W == 1
#include "pico/cyw43_arch.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    bool ssi_init();
    u16_t ssi_handler(int index, char *pcInsert, int insertLen, u16_t current_tag_part, u16_t *next_tag_part);

#ifdef __cplusplus
}
#endif

#endif // SSI_H

#endif // PICO_W
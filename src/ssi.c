#if PICO_W == 1

#include "ssi.h"

// these are basically get requests

// set SSI Tags
const char *ssi_tags[] = {
    "sysinfo"
};

bool ssi_init() {
    http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
    return true;
}

void datetime_to_json(datetime_t datetime, char *response, size_t response_size) {
    snprintf(response, response_size,
             "{\"year\":%d,\"month\":%d,\"day\":%d,\"dotw\":%d,\"hour\":%d,\"min\":%d,\"sec\":%d}",
             datetime.year, datetime.month, datetime.day, datetime.dotw,
             datetime.hour, datetime.min, datetime.sec);
}

void alarms_to_json(alarms_t alarms, char *response, size_t response_size) {
    char alarm_json[DATETIME_JSON_MAX_BUFFER_LENGTH];
    size_t offset = 0;

    offset += snprintf(response + offset, response_size - offset, "[");

    for (uint8_t i = 0; i < alarms.length; ++i) {
        datetime_to_json(alarms.alarms[i], alarm_json, sizeof(alarm_json));
        offset += snprintf(response + offset, response_size - offset, "%s%s", alarm_json, (i < alarms.length - 1) ? "," : "");
    }

    snprintf(response + offset, response_size - offset, "]");
}

u16_t ssi_handler(int index, char *pcInsert, int insertLen, u16_t current_tag_part, u16_t *next_tag_part) {
    if (index == 0) {
        const bool value = sys_info_get_gpio_value();
        const float temp = sys_info_get_temp();
        const uint32_t uptime = sys_info_get_uptime();
        char now[DATETIME_JSON_MAX_BUFFER_LENGTH];
        datetime_to_json(sys_info_get_time(), now, sizeof(now));

        alarms_t alarms;
        get_alarms(&alarms);
        // DATETIME_JSON_MAX_BUFFER_LENGTH + 1 for commas
        uint32_t size = (DATETIME_JSON_MAX_BUFFER_LENGTH + 1) * alarms.length + sizeof("[]");
        char alarms_json[size];
        alarms_to_json(alarms, alarms_json, sizeof(alarms_json));
        log_debug("alarm json size: %lu", size);

        uint8_t nextAlarmId = alarms.index == 0
            ? alarms.length - 1
            : alarms.index - 1;
        
        char response[SSI_RESPONSE_MAX_BUFFER_LENGTH];
        snprintf(
            response, sizeof(response),
            "{\"gpioValue\":%d,\"temp\":%.2f,\"uptime\":%lu,\"now\":%s,\"alarms\":%s, \"nextAlarm\": %d}",
            value, temp, uptime, now, alarms_json, nextAlarmId
        );
        log_debug("%s", response);
        log_debug("response size: %d", strlen(response));

        int data_len = strlen(response);
        if (current_tag_part * insertLen < data_len) {
            strncpy(pcInsert, response + current_tag_part * insertLen, insertLen);
            *next_tag_part = current_tag_part + 1;
            return (u16_t)insertLen;
        } else {
            *next_tag_part = 0xFFFF;
            return 0;
        }
    }

    return 0;
}

#endif
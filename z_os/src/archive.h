#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>

typedef struct {
    const char* item_id;
    const char* title;
    const char* author;
    const char* owner;
    const char* address;
    const char* postal_code;
    uint32_t reserve_nzd;
    int32_t latitude_fixed;
    int32_t longitude_fixed;
    const char* incident_date;
    const char* flight_path_convent;
    const char* flight_path_cemetery;
    const char* flight_path_exit;
    uint32_t color_profile_hex;
} __attribute__((packed)) HistoricalRecord;

// Linker maps this directly to the exported token inside archive.zig
extern const HistoricalRecord wingspread_archive;

#endif

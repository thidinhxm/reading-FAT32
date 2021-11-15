#ifndef __ENTRY_H__
#define __ENTRY_H__
#include <stdint.h>

#define LENGTH_FILE_NAME_1 10
#define LENGTH_FILE_NAME_2 12
#define LENGTH_FILE_NAME_3 4
 
struct MainEntry {
    uint8_t name[8];
    uint8_t extention[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t created_time[3];
    uint8_t created_date[2];
    uint8_t last_access_date[2];
    uint8_t first_cluster_high[2];
    uint8_t last_time_modified[2];
    uint8_t last_date_modified[2];
    uint8_t first_cluster_low[2];
    uint8_t size[4];
};

struct SubEntry {
    uint8_t sequence_number;
    uint8_t file_name_1[10]; // 5 character 2 bytes
    uint8_t attributes;
    uint8_t type;
    uint8_t check_sum;
    uint8_t file_name_2[12]; // NEXT 6 character 2 bytes 
    uint8_t must_be_zero[2];
    uint8_t file_name_3[4]; // NEXT 2 character 2 bytes
};

#endif
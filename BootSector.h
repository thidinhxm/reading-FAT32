#ifndef __BOOT_SECTOR_H__
#define __BOOT_SECTOR_H__
#include <stdint.h>
#include <windows.h>


#define SIZE_SECTORS_PER_CLUSTER 1
#define SIZE_RESERVED_SECTORS 2
#define SIZE_NUMBERS_OF_FAT 1
#define SIZE_TOTAL_SECTORS 4
#define SIZE_SECTORS_PER_FAT 4
#define SIZE_ROOT_CLUSTER 4
#define SIZE_SYSTEM_INFO_SECTOR 2
#define SIZE_BACKUP_BOOT_SECTOR 2
#define SIZE_FAT_TYPE 8


struct BootSector {
    uint8_t jump_code[3];
    uint8_t OEM_ID[8];
    uint8_t bytes_per_sector[2];
    uint8_t sectors_per_cluster;
    uint8_t reserved_sectors[SIZE_RESERVED_SECTORS];
    uint8_t numbers_of_FAT;
    uint8_t entry_of_RDET_unused[2];
    uint8_t total_sectors_unused[2];
    uint8_t media_descriptor;
    uint8_t sectors_per_FAT_unused[2];
    uint8_t sectors_per_track[2];
    uint8_t numbers_of_heads[2];
    uint8_t hidden_sectors[4];
    uint8_t total_sectors[SIZE_TOTAL_SECTORS];
    uint8_t sectors_per_FAT[SIZE_SECTORS_PER_FAT];
    uint8_t extended_flags[2];
    uint8_t version[2];
    uint8_t root_cluster[SIZE_ROOT_CLUSTER];
    uint8_t system_info_sector[SIZE_SYSTEM_INFO_SECTOR];
    uint8_t backup_boot_sector[SIZE_BACKUP_BOOT_SECTOR];
    uint8_t reserved_new_version[12];
    uint8_t physical_drive; // 0 (soft) OR 128 (hard)
    uint8_t reserved;
    uint8_t extended_signature;
    uint8_t serial_number[4];
    uint8_t volume_label[11];
    uint8_t FAT_type[SIZE_FAT_TYPE];
    uint8_t reserved_code[420];
    uint8_t end_signature[2];
};

void printInfoBootSector(const BootSector& bootSector); 
#endif
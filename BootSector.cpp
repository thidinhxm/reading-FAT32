#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "BootSector.h"
#include "helper.h"
using namespace std;

uint32_t convertBytesToInt(const BYTE bytes[], int n) {
    uint32_t res = 0;
    for (int i = n - 1; i >= 0; i--) {
        res = (res << 8) | bytes[i];
    }
    return res;
}

void printInfoBootSector(const BootSector& bootSector) {
    cout << "Sector per cluster: " << (int)bootSector.sectors_per_cluster << endl;
    cout << "Sectors of BootSector (Reserved Sectors): " << convertBytesToInt(bootSector.reserved_sectors, SIZE_RESERVED_SECTORS) << endl;
    cout << "Numbers of FAT:" << (int)bootSector.numbers_of_FAT << endl;
    cout << "Size of volume (sectors): " << convertBytesToInt(bootSector.total_sectors, SIZE_TOTAL_SECTORS) << endl;
    cout << "Size of FAT table (sectors): " << convertBytesToInt(bootSector.sectors_per_FAT, SIZE_SECTORS_PER_FAT) << endl;
    cout << "Root cluster: " << convertBytesToInt(bootSector.root_cluster, SIZE_ROOT_CLUSTER) << endl;
    cout << "System info sector: " << convertBytesToInt(bootSector.system_info_sector, SIZE_SYSTEM_INFO_SECTOR) << endl;
    cout << "Backup Boot sector: " << convertBytesToInt(bootSector.backup_boot_sector, SIZE_BACKUP_BOOT_SECTOR) << endl;
    cout << "FAT type: ";
    for (int i = 0; i < SIZE_FAT_TYPE; i++) {
        cout << bootSector.FAT_type[i];
    } 
    cout << endl;
}






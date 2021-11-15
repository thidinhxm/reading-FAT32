#include <iostream>
#include "BootSector.h"
#include "helper.h"

using namespace std;

void printInfoBootSector(const BootSector& bootSector) {
    wcout << "Sector per cluster: " << (int)bootSector.sectors_per_cluster << endl;
    wcout << "Sectors of BootSector (Reserved Sectors): " << convertBytesToInt(bootSector.reserved_sectors, SIZE_RESERVED_SECTORS) << endl;
    wcout << "Numbers of FAT:" << (int)bootSector.numbers_of_FAT << endl;
    wcout << "Size of volume (sectors): " << convertBytesToInt(bootSector.total_sectors, SIZE_TOTAL_SECTORS) << endl;
    wcout << "Size of FAT table (sectors): " << convertBytesToInt(bootSector.sectors_per_FAT, SIZE_SECTORS_PER_FAT) << endl;
    wcout << "Root cluster: " << convertBytesToInt(bootSector.root_cluster, SIZE_ROOT_CLUSTER) << endl;
    wcout << "System info sector: " << convertBytesToInt(bootSector.system_info_sector, SIZE_SYSTEM_INFO_SECTOR) << endl;
    wcout << "Backup Boot sector: " << convertBytesToInt(bootSector.backup_boot_sector, SIZE_BACKUP_BOOT_SECTOR) << endl;
    wcout << "FAT type: ";
    for (int i = 0; i < SIZE_FAT_TYPE; i++) {
        wcout << (wchar_t)bootSector.FAT_type[i];
    } 
    wcout << endl;
}






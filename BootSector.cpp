#include <iostream>
#include "BootSector.h"
#include "helper.h"

using namespace std;

void printInfoBootSector(const BootSector& boot_sector) {
    wcout << L"Số sector cho một cluster: " << (int)boot_sector.sectors_per_cluster << endl;
    wcout << L"Số sector vùng Boot Sector: " << convertBytesToInt(boot_sector.reserved_sectors, SIZE_RESERVED_SECTORS) << endl;
    wcout << L"Số lượng bảng FAT: " << (int)boot_sector.numbers_of_FAT << endl;
    wcout << L"Kích thước của volume (sectors): " << convertBytesToInt(boot_sector.total_sectors, SIZE_TOTAL_SECTORS) << endl;
    wcout << L"Kích thước mỗi bảng FAT(sectors): " << convertBytesToInt(boot_sector.sectors_per_FAT, SIZE_SECTORS_PER_FAT) << endl;
    wcout << L"Cluster bắt đầu của RDET: " << convertBytesToInt(boot_sector.root_cluster, SIZE_ROOT_CLUSTER) << endl;
    wcout << L"Sector chứa thông tin phụ (về cluster trống): " << convertBytesToInt(boot_sector.system_info_sector, SIZE_SYSTEM_INFO_SECTOR) << endl;
    wcout << L"Sector chứa bảng lưu của Boot Sector: " << convertBytesToInt(boot_sector.backup_boot_sector, SIZE_BACKUP_BOOT_SECTOR) << endl;
    wcout << L"Loại FAT: ";
    for (int i = 0; i < SIZE_FAT_TYPE; i++) {
        wcout << (wchar_t)boot_sector.FAT_type[i];
    } 
    wcout << endl;
}






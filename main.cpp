#include <iostream>
#include <io.h>
#include <vector>
#include <fcntl.h>
#include "BootSector.h"
#include "Entry.h"
#include "helper.h"
#include "Component.h"
#define _O_U16TEXT 0x20000
using namespace std;


int main() {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);

    BYTE sector[512];
    BootSector boot_sector;
    wchar_t disk_name;
    wcout << L"Nhập tên ô đĩa: ";
    wcin >> disk_name;
    WCHAR disk_path[] = L"\\\\.\\ :";
    disk_path[4] = disk_name;
    
    if (readSector(disk_path, 0, sector)) {
        wcout << L"Không thể đọc ổ cứng" << endl;
        return 1;
    }
    /*-----------------------------READ INFORMATION-------------------------------------------------------*/ 
    wcout << L"ĐỌC THÔNG TIN CỦA MỘT PHÂN VÙNG" << endl << endl;
    memcpy(&boot_sector, sector, sizeof(BootSector));
    printInfoBootSector(boot_sector);


    /* ---------------------CALCULATE AND GET MORE INFOMATION----------------------------------------------------*/

    uint32_t sb = convertBytesToInt(boot_sector.reserved_sectors, SIZE_RESERVED_SECTORS); // first sector of FAT1
    uint32_t nf = uint32_t(boot_sector.numbers_of_FAT); // numbers of FAT table
    uint32_t sf = convertBytesToInt(boot_sector.sectors_per_FAT, SIZE_SECTORS_PER_FAT);// sector per FAT
    uint32_t first_RDET = sb + nf * sf; // 32768 first sector of RDET
    uint32_t sc = (int)boot_sector.sectors_per_cluster;
    uint32_t first_cluster_RDET = convertBytesToInt(boot_sector.root_cluster, SIZE_ROOT_CLUSTER);

    uint32_t sector0 = first_RDET - first_cluster_RDET * sc; // sector cua cluster 0 tinh theo cluster bat dau RDET



    /* ------------------------READ ROOT FOLDER -------------------------------------------------------------*/ 
    wcout << L"\n------------------------------------------------" << endl;
    wcout << L"ĐỌC CÂY THƯ MỤC GỐC" << endl << endl;

    /* ----------------READ AND PRINT INFO FOLDER --------------------------*/
    readAndPrintFolderTree(disk_path, sector0, sc, first_cluster_RDET, sb, sf, 0);
    
    // for (auto bytes : fat1) {
    //     delete[] bytes;
    // }
    return 0;
}


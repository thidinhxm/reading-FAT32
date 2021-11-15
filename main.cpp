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
    BootSector bootSector;
    // wchar_t disk_name;
    // wcout << L"Nhập tên ô đĩa: " << endl;
    // wcin >> disk_name;
    // WCHAR disk_path[] = L"\\\\.\\ :";
    // disk_path[7] = disk_name;

    if (readSector(L"\\\\.\\H:", 0, sector)) {
        wcout << L"Không thể đọc ổ cứng" << endl;
        return 1;
    }
    wcout << L"ĐỌC THÔNG TIN CỦA MỘT PHÂN VÙNG" << endl << endl;
    memcpy(&bootSector, sector, sizeof(BootSector));
    printInfoBootSector(bootSector);


    /* --------------GET INFOMATION----------------------------------------------------*/

    uint32_t sb = convertBytesToInt(bootSector.reserved_sectors, SIZE_RESERVED_SECTORS); // first sector of FAT1
    uint32_t nf = uint32_t(bootSector.numbers_of_FAT); // numbers of FAT table
    uint32_t sf = convertBytesToInt(bootSector.sectors_per_FAT, SIZE_SECTORS_PER_FAT);// sector per FAT
    uint32_t first_RDET = sb + nf * sf; // 32768 first sector of RDET
    uint32_t sc = (int)bootSector.sectors_per_cluster;
    uint32_t first_cluster_RDET = convertBytesToInt(bootSector.root_cluster, SIZE_ROOT_CLUSTER);

    uint32_t sector0 = first_RDET - first_cluster_RDET * sc; // sector cua cluster 0 tinh theo cluster bat dau RDET

    wcout << L"------------------------------------------------" << endl;
    wcout << L"ĐỌC CÂY THƯ MỤC GỐC" << endl << endl;

    // Read FAT1
    vector<BYTE*> fat1;
    while (true) {
        readSector(L"\\\\.\\H:", sb * 512, sector);
        int i = 0;
        while (i < 509)  {
            BYTE* fat_member = new BYTE[4];
            memcpy(fat_member, sector + i, 4);
            if (convertBytesToInt(fat_member, 4) == 0) {
                delete[] fat_member;
                break;
            }
            fat1.push_back(fat_member);
            i += 4;
        }
        if (i < 508) {
            break;
        }
        sb++;
    }
    
    readAndPrintFolderInfo(L"\\\\.\\H:", fat1, sector0, sc, first_cluster_RDET);
    
    for (auto bytes : fat1) {
        delete[] bytes;
    }
    return 0;
}


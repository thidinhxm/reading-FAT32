#include <iostream>
#include <io.h>
#include <vector>
#include <fcntl.h>
#include "BootSector.h"
#include "Sector.h"
#include "Entry.h"
#include "helper.h"
#include "Component.h"
#define _O_U16TEXT 0x20000
using namespace std;


int main() {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << sizeof(MainEntry) << endl;
    wcout << sizeof(SubEntry) << endl;
    BYTE sector[512];
    BootSector bootSector;
    if (readSector(L"\\\\.\\H:", 0, sector)) {
        wcout << "Cannot read disk" << endl;
        return 1;
    }
    
    memcpy(&bootSector, sector, sizeof(BootSector));
    printInfoBootSector(bootSector);
    uint32_t sb = convertBytesToInt(bootSector.reserved_sectors, SIZE_RESERVED_SECTORS); // first sector of FAT1
    uint32_t nf = uint32_t(bootSector.numbers_of_FAT); // numbers of FAT table
    uint32_t sf = convertBytesToInt(bootSector.sectors_per_FAT, SIZE_SECTORS_PER_FAT);// sector per FAT
    uint32_t first_RDET = sb + nf * sf;
    wcout << "Location of first sector of RDET: " << first_RDET << endl;
    
    wcout << endl << endl << endl;
    wcout << L"Đọc cây thư mục gốc" << endl << endl;
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
    vector<SubEntry> subEntryList;
    // read RDET
    for (int i = 1;; i++) { // handle each sector start in RDET
        readSector(L"\\\\.\\H:", first_RDET * 512 * i, sector);
        if (sector[0] == 0) {
            break;
        }
        
        for (int j = 0; j < 512; j += 32) { // handle each entry, 1 entry = 32 bytes
            if (sector[j] == 0) { // empty entry --> end
                break;
            }
            if (sector[j] == 0xe5) { // e5h ---> deleted
                subEntryList.clear();
                continue;
            }
            
            if (sector[j + 11] == 0x0f) { // 0fh ---> subEntry
                SubEntry subEntry;
                memcpy(&subEntry, sector + j, 32);
                subEntryList.push_back(subEntry);
            } 
            else if (sector[j + 11] == 0x10 || sector[j + 11] == 0x20) { // 10h = 1 0000b ----> folder 20h = 10 000 ---> file
                MainEntry mainEntry;
                memcpy(&mainEntry, sector + j, 32);
                Component component;
                if(!subEntryList.size()) { // there are no sub entrys
                    setInfo(component, mainEntry, fat1);
                    
                }
                else {
                    setInfo(component, mainEntry, subEntryList, fat1);
                }
                printInfo(component);
            }
            else {
                subEntryList.clear();
            }
        }
    }
    
    
    for (auto bytes : fat1) {
        delete[] bytes;
    }
    return 0;
}


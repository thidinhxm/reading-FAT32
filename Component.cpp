#include "Component.h"
#include "helper.h"

/*--------- SET INFORMATION FOR FILE OR FOLDER IF IT JUST HAS MAIN ENTRY--------------------*/
void setInfo(Component& component, const MainEntry& mainEntry, vector<BYTE*> fat1) {
    wstring name = L"";
    for (int i = 0; i < 8; i++) {
        name += (wchar_t)mainEntry.name[i];
    }     
    component.name = trim(name);

    if (mainEntry.attributes == 0x20) {
        component.attributes = L"Tập tin";
        component.name += L'.';
        for (int i = 0; i < 3; i++) {
            component.name += mainEntry.extention[i];
        }
    }
    else {
        component.attributes = L"Thư mục";
    }

    component.first_cluster = convertBytesToInt(mainEntry.first_cluster_high, 2) * 0x100 + convertBytesToInt(mainEntry.first_cluster_low, 2); 
    
    
    if (component.first_cluster != 0) { // first_cluster = 0 ==> there are no clusters
        component.clusters.push_back(component.first_cluster);

        for (int i = component.first_cluster; i < fat1.size(); i++) { // read FAT1 to know next clusters
            uint32_t index_cluster = convertBytesToInt(fat1[i], 4);
        if (index_cluster == 0x0fffffff) {
            break;
        }
        else {
            component.clusters.push_back(index_cluster);
            }
        }
    }
    component.size = convertBytesToInt(mainEntry.size, 4);
}


/*--------- SET INFORMATION FOR FILE OR FOLDER IF IT HAS MAIN ENTRY AND ONE OR MORE SUB ENTRY--------------------*/
void setInfo(Component& component, const MainEntry& mainEntry, const vector<SubEntry> subEntryList, vector<BYTE*> fat1) {
    wstring name = L"";
    for (int i = subEntryList.size() - 1; i >= 0; i--) { 
        name += convertBytesToWString(subEntryList[i].file_name_1, LENGTH_FILE_NAME_1);
        name += convertBytesToWString(subEntryList[i].file_name_2, LENGTH_FILE_NAME_2);
        name += convertBytesToWString(subEntryList[i].file_name_3, LENGTH_FILE_NAME_3);
    }
    
    component.name = name;
    if (mainEntry.attributes == 0x20) {
        component.attributes = L"Tập tin";
    }
    else {
        component.attributes = L"Thư mục";
    }

    component.first_cluster = convertBytesToInt(mainEntry.first_cluster_high, 2) * 0x100 + convertBytesToInt(mainEntry.first_cluster_low, 2); 
    
    
    if (component.first_cluster != 0) { // first_cluster = 0 ==> there are no clusters
        component.clusters.push_back(component.first_cluster);

        for (int i = component.first_cluster; i < fat1.size(); i++) {
            uint32_t index_cluster = convertBytesToInt(fat1[i], 4);
        if (index_cluster == 0x0fffffff) {
            break;
        }
        else {
            component.clusters.push_back(index_cluster);
            }
        }
    }
    component.size = convertBytesToInt(mainEntry.size, 4);
}


/*-------------------PRINT INFORMATION OF FILE OR FOLDER ---------------------------------*/
void printInfo(const Component& component, uint32_t sector0, uint32_t sc, int tab) {
    printTab(tab);
    wcout << L"Tên: " << component.name << endl;
    printTab(tab);
    wcout << L"Trạng thái: " << component.attributes << endl;
    
    if (component.first_cluster != 0) {
        printTab(tab);
        wcout << L"Cluster bắt đầu: " << component.first_cluster << endl;
        printTab(tab);
        wcout << L"Chiếm các cluster: ";
        for (int cluster : component.clusters) {
            wcout << cluster << L" ";
        }
        wcout << endl;
        printTab(tab);
        wcout << L"Chiếm các sector:" << endl;
        printTab(tab);
        for (int cluster : component.clusters) {
            wcout << cluster * sc + sector0 << L" --> " << cluster * sc + sector0 + sc - 1 << L", ";
        }
        wcout << endl;
    }
    else {
        printTab(tab);
        wcout << L"Không chiếm cluster nào" << endl;
    }
    printTab(tab);
    wcout << L"Kích thước: " << component.size << endl;
}


/*------------------------READ AND PRINT FOLDER INFORMATION INCLUDE SUB FOLDERS AND FILES ------------------------------------------------*/ 
void readAndPrintFolderInfo(LPCWSTR disk_path, const vector<BYTE*>& fat1, uint32_t sector0, uint32_t sc, uint32_t root_cluster, int tab) {// tab ==> display better
    vector<SubEntry> subEntryList;
    BYTE sector[512];
    uint32_t first_sector = sector0 + sc * root_cluster; // first sector of file or folder ==> sector ~ root_cluster
    for (int i = 1;; i++) { // handle each sector start in RDET
        readSector(disk_path, (first_sector + i - 1) * 512, sector);
        if (sector[0] == 0) {
            break;
        }
        
        for (int j = 0; j < 512 - 31; j += 32) { // handle each entry, 1 entry = 32 bytes
            if (sector[j] == 0) { // empty entry --> end
                break;
            }
            if (i == 1 && j < 33) { // ignore first 2 entry
                continue;
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
            else if (sector[j + 11] != 0x10 && sector[j + 11] != 0x20) { // 10h = 1 0000b --> folder 20h = 10 000--> file -->  !folder && !file
                subEntryList.clear();
            }
            else {
                MainEntry mainEntry;
                memcpy(&mainEntry, sector + j, 32);
                Component component;
                if(!subEntryList.size()) { // there are no sub entrys
                    setInfo(component, mainEntry, fat1);      
                }
                else {
                    setInfo(component, mainEntry, subEntryList, fat1);
                }
                subEntryList.clear(); // clear sub entry for next main entry

                printInfo(component, sector0, sc, tab);
                

                if (sector[j + 11] == 0x10) { // folder ==> read sub folder
                    readAndPrintFolderInfo(disk_path, fat1, sector0, sc, component.first_cluster, tab + 3);
                }
                else {// file ---> print information of file txt
                    wstring extension = component.name.substr(component.name.size() - 3, 3); // 3 last characters
                    if (extension == L"txt" || extension == L"TXT") {
                        readAndPrintTxtFile(disk_path, component, sector0, sc , tab);
                    }
                    else {
                    printTab(tab);
                    wcout << L"Không phải file txt => cần một phần mềm chuyên dụng để mở" << endl;
                    }
                }
                wcout << endl << endl << endl << endl;
            }
        }
    }
}

void readAndPrintTxtFile(LPCWSTR disk_path, const Component& file, uint32_t sector0, uint32_t sc, int tab) {
    BYTE sector[512];
    wstring result;
    for (int cluster : file.clusters) {
        uint32_t first_sector = sector0 + sc * cluster;
        int j = 0; 
        for (j; j < sc; j++) {
            readSector(disk_path, (first_sector + j) * 512, sector);
            int i = 0;
            for (i; i < 512; i++) {
                if (sector[i] == 0) {
                    break;
                }
                if (sector[i] < 0x80) { // unicode 1 byte
                    result += (wchar_t)sector[i];
                }
                else if (0xc2 <= sector[i] && sector[i] <= 0xdf) { // unicode 2 byte
                    int number = (sector[i] - 0xc2) * 64 + sector[i + 1]; // sau mỗi 64 số thì tăng chỉ sô nhận biết lên 1, mò bảng unicode rồi tính ra được :) 
                    wchar_t wch = (wchar_t)(number);
                    result += wch;
                    ++i;
                }
                else if (0xe0 <= sector[i] && sector[i] <= 0xef) { // unicode 3 bytes
                    int number = (sector[i] - 0xe0) * 0x1000 + (sector[i + 1] - 0x80) * 64 + (sector[i + 2] - 0x80); // mò bảng unicode rồi tính ra được :) 
                    wchar_t wch = (wchar_t)(number);
                    result += wch;
                    i += 2;
                }
                // kí tự bàn phím chỉ có 3 byte utf8 là tối đa
                
            }
            if (i < 512) {
                break;
            }
        }
        if (j < sc) {
            break;
        }
    }
    wcout << L"Nội dung của tập tin txt" << endl;
    wcout << result << endl;
}
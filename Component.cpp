#include "Component.h"
#include "helper.h"

/*--------- READ MAIN ENNTRY, SUB ENTRY AND FAT1 TO GET FILE OR FOLDER --------------------*/
Component getComponent(LPCWSTR disk_path, const MainEntry& main_entry, const vector<SubEntry>& sub_entry_list, int sb, int sf) {
    Component component; 
    wstring name = L"";
    if (sub_entry_list.size() == 0) { // no sub entry ==> just has main entry
        for (int i = 0; i < 8; i++) {
            name += (wchar_t)main_entry.name[i];
        }     
        component.name = trim(name);

        if (main_entry.attributes == 0x20) {
            component.attributes = L"Tập tin";
            component.name += L'.';
            for (int i = 0; i < 3; i++) {
                component.name += main_entry.extention[i];
            }
        }
        else {
            component.attributes = L"Thư mục";
        }
    }
    else { // have sub entry
        for (int i = sub_entry_list.size() - 1; i >= 0; i--) { 
            name += convertBytesToWString(sub_entry_list[i].file_name_1, LENGTH_FILE_NAME_1);
            name += convertBytesToWString(sub_entry_list[i].file_name_2, LENGTH_FILE_NAME_2);
            name += convertBytesToWString(sub_entry_list[i].file_name_3, LENGTH_FILE_NAME_3);
        }    
        component.name = name;
        if (main_entry.attributes == 0x20) {
            component.attributes = L"Tập tin";
        }
        else {
            component.attributes = L"Thư mục";
        }
    }

    component.first_cluster = convertBytesToInt(main_entry.first_cluster_high, 2) * 0x100 + convertBytesToInt(main_entry.first_cluster_low, 2); 
    
    
    if (component.first_cluster != 0) { // first_cluster = 0 ==> there are no clusters
        component.clusters = getClusters(disk_path, component.first_cluster, sb, sf);
    }
    component.size = convertBytesToInt(main_entry.size, 4);

    return component;
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
void readAndPrintFolderTree(LPCWSTR disk_path, uint32_t sector0, uint32_t sc, uint32_t root_cluster, int sb, int sf, int tab) {// tab ==> display better
    vector<SubEntry> sub_entry_list;
    BYTE sector[512];
    uint32_t first_sector = sector0 + sc * root_cluster; // first sector of file or folder ==> sector ~ root_cluster
    for (int i = 0;; i++) { // handle each sector start in RDET
        readSector(disk_path, (first_sector + i) * 512, sector);
        if (sector[0] == 0) {
            break;
        }
        for (int j = 0; j < 512 - 31; j += 32) { // handle each entry, 1 entry = 32 bytes
            if (sector[j] == 0) { // empty entry --> end
                break;
            }
            if (i == 0 && j < 33) { // ignore first 2 entry ==> system
                continue;
            }
            if (sector[j] == 0xe5) { // e5h ---> deleted
                sub_entry_list.clear();
                continue;
            }
            
            if (sector[j + 11] == 0x0f) { // 0fh ---> subEntry
                SubEntry sub_entry;
                memcpy(&sub_entry, sector + j, 32);
                sub_entry_list.push_back(sub_entry);
            } 
            else if (sector[j + 11] != 0x10 && sector[j + 11] != 0x20) { // !folder && !file
                sub_entry_list.clear();
            }
            else {// folder || file
                MainEntry main_entry;
                memcpy(&main_entry, sector + j, 32);

                Component component = getComponent(disk_path, main_entry, sub_entry_list, sb, sf);
                printInfo(component, sector0, sc, tab);
                sub_entry_list.clear(); // clear sub entry for next main entry

                if (sector[j + 11] == 0x10) { // folder ==> read sub folder
                    readAndPrintFolderTree(disk_path, sector0, sc, component.first_cluster, sb, sf, tab + 3);
                }
                else {// file ---> print information of file txt
                    handleComponentIsFile(disk_path, component, sector0, sc, tab);
                }
                wcout << endl << endl << endl << endl;
            }
        }
    }
}

/*-------------- PRINT CONTENT IF TYPE FILE IS TXT, ELSE, PRINT NOTIFICATION,------------------------------*/ 
void handleComponentIsFile(LPCWSTR disk_path, const Component& component, uint32_t sector0, int sc, int tab) {
    wstring extension = component.name.substr(component.name.size() - 3, 3); // 3 last characters
    if (extension == L"txt" || extension == L"TXT") {
        vector<BYTE> bytes = readTXTFile(disk_path, component, sector0, sc, tab);
        printTXTFile(bytes, tab);
    }
    else {
        printTab(tab);
        wcout << L"Không phải file txt => cần một phần mềm chuyên dụng để mở" << endl;
    }
}

/* --------------------- READ FILE TXT AND RETURN VECTOR BYTE----------------------------------------------*/ 
vector<BYTE> readTXTFile(LPCWSTR disk_path, const Component& file, uint32_t sector0, uint32_t sc, int tab) {
    BYTE sector[512];
    vector<BYTE> bytes;
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
                bytes.push_back(sector[i]);
            }
            if (i < 512) {
                break;
            }
        }
        if (j < sc) {
            break;
        }
    }
    return bytes;
}

void printTXTFile(vector<BYTE> bytes) {
    wcout << L"Nội dung của tập tin txt trên" << endl;
    wcout << convertUnicodeHexCodeToCodePoint(bytes) << endl;
}

void printTXTFile(vector<BYTE> bytes, int tab) { // print better than function same name
    printTab(tab);
    wcout << L"Nội dung của tập tin txt trên" << endl << endl;
    wstring result = convertUnicodeHexCodeToCodePoint(bytes);
    printTab(tab);
    for (int i = 0; i < result.size(); i++) {
        wcout << result[i];
        if (result[i] == 0x0a) { // 0ah is end line
            printTab(tab);
        }
    }
}



vector<int> getClusters(LPCWSTR disk_path, int first_cluster, int sb, int sf) { // read FAT1 and get all cluster
    vector<int> clusters;
    BYTE sector[512];
    // BYTE fat_member[4];
    clusters.push_back(first_cluster);
    int started_read_sector = first_cluster / 128 + sb;  // 1 sector has 512/4 = 128 fat members, 1 fat_member represent 1 cluster
    int cluster = first_cluster;
    for (int i = started_read_sector; i < sf + sb; i++) {
        readSector(disk_path, i * 512, sector);
        while (true) {
            int read_point = cluster % 128;
            cluster = convertBytesToInt(sector + read_point * 4, 4);
            
            if (cluster == 0x0fffffff) {
                break;
            }
            else {
                clusters.push_back(cluster);
                if (cluster % 128 == 0) {
                    break;
                }
            }        
        }

        if (cluster == 0X0fffffff) {
            break;
        }
    }
    return clusters;
}



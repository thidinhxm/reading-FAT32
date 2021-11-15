#include "Component.h"
#include "helper.h"

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

    component.first_cluster = convertBytesToInt(mainEntry.first_cluster_high, 2) * 100 + convertBytesToInt(mainEntry.first_cluster_low, 2); 
    
    for (int i = component.first_cluster; i < fat1.size(); i++) {
        uint32_t index_cluster = convertBytesToInt(fat1[i], 4);
        if (index_cluster == 0x0fffffff) {
            component.clusters.push_back(i);
            break;
        }
        else {
            component.clusters.push_back(index_cluster);
        }
    }
    component.size = convertBytesToInt(mainEntry.size, 4);
}

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

    component.first_cluster = convertBytesToInt(mainEntry.first_cluster_high, 2) * 100 + convertBytesToInt(mainEntry.first_cluster_low, 2); 
    for (int i = component.first_cluster; i < fat1.size(); i++) {
        uint32_t index_cluster = convertBytesToInt(fat1[i], 4);
        if (index_cluster == 0x0fffffff) {
            component.clusters.push_back(i);
            break;
        }
        else {
            component.clusters.push_back(index_cluster);
        }
    }
    component.size = convertBytesToInt(mainEntry.size, 4);
}

void printInfo(const Component& component) {
    wcout << L"Tên: " << component.name << endl;
    wcout << L"Trạng thái: " << component.attributes << endl;
    wcout << L"Cluster bắt đầu: " << component.first_cluster << endl;
    wcout << L"Chiếm các cluster: ";
    for (int cluster : component.clusters) {
        wcout << cluster << " ";
    }
    wcout << endl;
    wcout << L"Kích thước: " << component.size << endl;
}

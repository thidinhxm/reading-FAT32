#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <iostream>
#include <string>
#include <vector>
#include "Entry.h"
#include <windows.h>
using namespace std;

struct Component{
    wstring name;
    wstring attributes;
    uint32_t size;
    int first_cluster;
    vector<int> clusters;
};

Component getComponent(LPCWSTR disk_path, const MainEntry& main_entry, const vector<SubEntry>& sub_entry_list, int sb, int sf);
void printInfo(const Component& component,  uint32_t sector0, uint32_t sc, int tab);
void readAndPrintFolderTree(LPCWSTR disk_path, uint32_t sector0, uint32_t sc, uint32_t root_cluster, int sb, int sf, int tab);
vector<BYTE> readTXTFile(LPCWSTR disk_path, const Component& file, uint32_t sector0, uint32_t sc, int tab);
void printTXTFile(vector<BYTE> bytes);
void printTXTFile(vector<BYTE> bytes, int tab);
void handleComponentIsFile(LPCWSTR disk_path, const Component& component, uint32_t sector0, int sc, int tab);
vector<int> getClusters(LPCWSTR disk_path, int first_cluster, int sb, int sf);



#endif
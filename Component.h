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


void setInfo(Component& component, const MainEntry& mainEntry, const vector<SubEntry> subEntryList, vector<BYTE*> fat1);
void setInfo(Component& component, const MainEntry& mainEntry, vector<BYTE*> fat1);
void printInfo(const Component& component,  uint32_t sector0, uint32_t sc, int tab);
void readAndPrintFolderInfo(LPCWSTR disk_path, const vector<BYTE*>& fat1, uint32_t sector0, uint32_t sc, uint32_t root_cluster, int tab);
void readTxtFile(LPCWSTR disk_path, const vector<BYTE*>& fat1, uint32_t sector0, uint32_t sc, uint32_t root_cluster, int tab);
#endif
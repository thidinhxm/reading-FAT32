#ifndef __DATA_H__
#define __DATA_H__

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
void printInfo(const Component& component);

#endif
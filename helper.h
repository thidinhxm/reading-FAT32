#ifndef __HELPER_H__
#define __HELPER_H__
#include <string>
#include <stdint.h>
#include <windows.h>
using namespace std;

uint32_t convertBytesToInt(const uint8_t bytes[], int n);
wstring trim(const wstring& src);
wstring convertBytesToWString(const uint8_t bytes[], int n);
int readSector(LPCWSTR drive, int readPoint, BYTE sector[512]);
void printTab(int tab);
#endif
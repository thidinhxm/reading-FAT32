#ifndef __HELPER_H__
#define __HELPER_H__
#include <string>
#include <stdint.h>
#include <windows.h>
#include <vector>
using namespace std;

uint32_t convertBytesToInt(const uint8_t bytes[], int n);
wstring trim(const wstring& src);
wstring convertBytesToWString(const uint8_t bytes[], int n);
int readSector(LPCWSTR drive, int readPoint, BYTE sector[512]);
wstring convertUnicodeHexCodeToCodePoint(vector<BYTE> bytes);   // chuyển utf8 thành dạng 2 byte để hiển thị
void printTab(int tab);
#endif
#ifndef __HELPER_H__
#define __HELPER_H__
#include <string>
#include <stdint.h>
using namespace std;

wstring toHex(unsigned int decnum);
uint32_t convertBytesToInt(const uint8_t bytes[], int n);
wstring trim(const wstring& src);
wstring convertBytesToWString(const uint8_t bytes[], int n);
#endif
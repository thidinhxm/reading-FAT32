#include "helper.h"
#include <iostream>
#include <vector>
uint32_t convertBytesToInt(const uint8_t bytes[], int n) {
    uint32_t res = 0;
    for (int i = n - 1; i >= 0; i--) {
        res = (res << 8) | bytes[i];
    }
    return res;
}

wstring trim(const wstring& src) {
    int left = 0; 
    int right = src.size() - 1;
    for (int i = 0; i < src.size(); i++) {
        if (src[i] != L' ') {
            left = i;
            break;
        }
    }

    for (int i = src.size() - 1; i >= 0; i--) {
        if (src[i] != L' ') {
            right = i;
            break;
        }
    }
    return src.substr(left, right - left + 1);
}

wstring convertBytesToWString(const uint8_t bytes[], int n) {
    wstring str = L"";
    for (int i = 0; i < n - 1; i += 2) {
        if (bytes[i] == 0xff && bytes[i + 1] == 0xff) {
            break;
        }
        if (bytes[i] == 0 && bytes[i + 1] == 0) {
            break;
        }
        str += (wchar_t)convertBytesToInt(bytes + i, 2);
    }
    return str;
}

int readSector(LPCWSTR drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFileW(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void printTab(int tab) {
    for (int i = 0; i < tab; i++) {
        wcout << '\t';
    }
}


wstring convertUnicodeHexCodeToCodePoint(vector<BYTE> bytes) {
    wstring result = L"";
    for (int i = 0; i < bytes.size(); i++) {
        if (bytes[i] < 0x80) { // unicode 1 byte
            result += (wchar_t)bytes[i];
        }
        else if (0xc2 <= bytes[i] && bytes[i] <= 0xdf) { // unicode 2 byte
            int number = (bytes[i] - 0xc2) * 64 + bytes[i + 1]; // sau m???i 64 s??? th?? t??ng ch??? s?? nh???n bi???t l??n 1, m?? b???ng unicode r???i t??nh ra ???????c :) 
            wchar_t wch = (wchar_t)(number);
            result += wch;
            ++i;
        }
        else if (0xe0 <= bytes[i] && bytes[i] <= 0xef) { // unicode 3 bytes
            int number = (bytes[i] - 0xe0) * 0x1000 + (bytes[i + 1] - 0x80) * 64 + (bytes[i + 2] - 0x80); // m?? b???ng unicode r???i t??nh ra ???????c :) 
            wchar_t wch = (wchar_t)(number);
            result += wch;
            i += 2;
        }
    }
    return result;
}
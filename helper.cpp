#include "helper.h"

wstring toHex(unsigned int decnum) {
    wstring hexnum = L"";
    int rem;
    while(decnum != 0)
    {
        rem = decnum % 16;
        if(rem < 10)
            rem = rem + '0';
        else
            rem = rem - 10 + 'A';
        hexnum = (wchar_t)rem + hexnum;
        decnum = decnum / 16;
    }
    return hexnum.size() >= 2 ? hexnum : hexnum.size() == 1 ? L"0" + hexnum : L"00";
}

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
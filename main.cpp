#include <iostream>
// #include "helper.h"
#include "BootSector.h"
#include "Sector.h"
using namespace std;
int main() {
    BYTE sector[512];
    BootSector bootSector;
    if (readSector(L"\\\\.\\H:",0, sector)) {
        cout << "Cannot read disk" << endl;
    }
    else {
        memcpy(&bootSector, sector, sizeof(BootSector));
        printInfoBootSector(bootSector);
    }
    return 0;
}

#include <global.h>

uint8_t mainMode = DEF;

struct note initPinNotes[][3] =
{ //pin, note, snote
    {2, 36, 47},
    {3, 37, 48},
    {4, 38, 49},
    {5, 39, 50},
    {6, 40, 51},
    {7, 41, 52},
    {8, 42, 53}
};

uint8_t getMainMode() {
    return mainMode;
}

void setMainMode(uint8_t mode) {
    mainMode = mode;
}



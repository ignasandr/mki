#include <global.h>

using namespace std;

uint8_t mainMode = DEF;

vector<uint8_t> arpSequence;

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

void addToArp(uint8_t pin) {
    if(find(arpSequence.begin(), arpSequence.end(), pin) == arpSequence.end()) {
        arpSequence.push_back(pin);
    }
    printVec(arpSequence);
}

void removeFromArp(uint8_t pin) {
    arpSequence.erase(std::remove(arpSequence.begin(), arpSequence.end(), pin), arpSequence.end());
    printVec(arpSequence);
}

void printVec(vector<uint8_t> &vect) {
    Serial.print("-");
    for(auto v : vect) {
        Serial.print(v);
    }
    Serial.println("-");
}
#include <output.h>

using namespace std;

vector<uint8_t> arpSequence;

void manageArpSequence(uint8_t pin, uint8_t change) {
    switch(change) {
        case PRESSED:
            if(find(arpSequence.begin(), arpSequence.end(), pin) == arpSequence.end()) {
                arpSequence.push_back(pin);
            }
            printVec(arpSequence);
            break;
        case RELEASED:
            arpSequence.erase(std::remove(arpSequence.begin(), arpSequence.end(), pin), arpSequence.end());
            printVec(arpSequence);
            break;
    }
}

void printVec(vector<uint8_t> &vect) {
    Serial.print("-");
    for(auto v : vect) {
        Serial.print(v);
    }
    Serial.println("-");
}
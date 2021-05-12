#include <global.h>

using namespace std;

uint8_t mainMode = DEF;

boolean intClock = false;

uint8_t noteChan = 2;
uint8_t sampChan = 5;

vector<uint8_t> arpSequence;
uint8_t playhead = 0;

uint8_t divisionTicks = 12; // 12 for 8th notes, 6 for 16th notes

const uint16_t maxCooldown[] = {4, 1, 1600};
uint16_t currentCooldown[] = {4, 1, 1600}; // starting cooldown values for DEF, SAMP, HOLD

const uint8_t minVelocity[] = {80, 127, 20};
const uint8_t maxVelocity[] = {117, 127, 110};

struct note pinNotes[][3] =
{ //pin, note, snote
    {2, 36, 100},
    {3, 38, 101},
    {4, 41, 102},
    {5, 43, 103},
    {6, 46, 104},
    {7, 47, 105},
    {8, 48, 106}
};

uint8_t getMainMode() {
    return mainMode;
}

void setMainMode(uint8_t mode) {
    mainMode = mode;
}

boolean getIntClock() {
    return intClock;
}

void intClockOn() {
    intClock = true;
}

void intClockOff() {
    intClock = false;
}



void addToArp(uint8_t pin) {
    if(find(arpSequence.begin(), arpSequence.end(), pin) == arpSequence.end()) {
        arpSequence.push_back(pin);
    }
    // printVec(arpSequence);
}

void removeFromArp(uint8_t pin) {
    arpSequence.erase(std::remove(arpSequence.begin(), arpSequence.end(), pin), arpSequence.end());
    // printVec(arpSequence);
}

uint8_t getArpSize() {
    return arpSequence.size();
}

uint8_t getFromArp(uint8_t id) {
    return arpSequence[id];
}

uint8_t getPlayheadPos() {
   return playhead; 
}


// playhead actions

void incrPlayhead() {
    playhead += 1;
}

void resetPlayhead() {
    playhead = 0;
}

// currently playing actions

uint8_t currentlyPlaying = 0;

uint8_t getCurrentlyPlaying() {
    return currentlyPlaying;
}

void addToCurrentlyPlaying(uint8_t note) {
    currentlyPlaying = note;
}

void clearCurrentlyPlaying() {
    currentlyPlaying = 0;
}

// utils

void printVec(vector<uint8_t> &vect) {
    Serial.print("-");
    for(auto v : vect) {
        Serial.print(v);
    }
    Serial.println("-");
}

uint8_t getNoteByPin(uint8_t pin) {
    for(auto arr : pinNotes) {
        if(arr->pin == pin) {
            return arr->note;
        }
    }
    return 0;
}

uint8_t getSnoteByPin(uint8_t pin) {
    for(auto arr : pinNotes) {
        if(arr->pin == pin) {
            return arr->snote;
        }
    }
    return 0;
}

uint16_t getRotaryValue() {
    uint16_t value = analogRead(0);
    return value;
}

uint8_t getMappedRotaryValue() {
    return map(getRotaryValue(), 0, 1024, 1, getDivisionTicks());
}


uint8_t getNoteChan() {
    return noteChan;
}

uint8_t getSampChan() {
    return sampChan;
}

// Arp stuff (2)


uint8_t getDivisionTicks() {
    return divisionTicks;
}

void setDivisionTicks(uint8_t numberOfTicks) {
    divisionTicks = numberOfTicks;
}

// stop counter

boolean stopCounterOn = false;
uint8_t noteToStop = 0;
uint8_t noteToStopChan = 0;

boolean getStopCounterOn() {
    return stopCounterOn;
}

void turnStopCounterOn(uint8_t note, uint8_t chan, uint8_t numberOfTicks) {
    stopCounterOn = true;
    setStopCounter(numberOfTicks);
    noteToStop = note;
    noteToStopChan = chan; 
}

void turnStopCounterOff() {
    stopCounterOn = false;
    noteToStop = 0;
    noteToStopChan = 0;
}

uint8_t getNoteToStop() {
    return noteToStop;
}

uint8_t getNoteToStopChan() {
    return noteToStopChan;
}

uint8_t stopCounter = 0;

uint8_t getStopCounter() {
    return stopCounter;
}

void decrStopCounter() {
    stopCounter -= 1; 
}

void setStopCounter(uint8_t numberOfTicks) {
    stopCounter = numberOfTicks;
}

// Cooldown stuff 


uint16_t getCurrentCooldown(uint8_t mode) {
    return currentCooldown[mode];
}

void incrCurrentCooldown(uint8_t mode) {
    currentCooldown[mode] += 1;
}

void decrCurrentCooldown(uint8_t mode) {
    currentCooldown[mode] -= 1;
}

uint16_t getMaxCooldown(uint8_t mode) {
    return maxCooldown[mode];
}

// Hold note stuff

boolean holdAutoDecrOn = false;

boolean getHoldAutoDecrOn() {
    return holdAutoDecrOn;
}

void turnHoldAutoDecrOn() {
    holdAutoDecrOn = true;
}

void turnHoldAutoDecrOff() {
    holdAutoDecrOn = false;
}

// Calculate velocity

uint8_t calculateVelocity(uint8_t mode) {
    uint8_t velocity = 1;
    velocity = map(getCurrentCooldown(mode), 1, getMaxCooldown(mode), minVelocity[mode], maxVelocity[mode]);
    return velocity;
    // switch(getMainMode()) {
    //     case(DEF):
    //         velocity = map(getCurrentCooldown(DEF), 0, getMaxCooldown(DEF), 70, getCurrentCooldown(DEF));
    //         break;
    //     case(HOLD):
    // }
}

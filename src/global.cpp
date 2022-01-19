#include <global.h>

using namespace std;

uint8_t mainMode = DEF;

boolean intClock = false;

uint8_t boxNumber = 1; // 

uint8_t noteChan0 = 2; // used with boxNumber = 0;
uint8_t noteChan1 = 3; // used with boxNumber = 1;
uint8_t seqChan = 4;
uint8_t sampChan = 5;

vector<uint8_t> arpSequence;
uint8_t playhead = 0;

uint8_t seqPlayhead = 0;

uint8_t divisionTicks = 12; // 12 for 8th notes, 6 for 16th notes

boolean cooldownClock[] = {false, false, false}; // DEF, SAMP, HOLD

uint16_t cooldownTime[] = {0, 0, 0};
uint16_t cooldownThreshold[] = {300, 6000, 6000}; //time after which 1 unit of cooldown is replenished for {DEF, SAMP, SEQ}


const uint16_t maxCooldown[] = {8, 1, 1};
uint16_t currentCooldown[] = {8, 1, 1}; // starting cooldown values for DEF, SAMP, SEQ

const uint8_t minVelocity[] = {80, 127, 20};
const uint8_t maxVelocity[] = {116, 127, 110};

struct note pinNotes0[][6] =
{ //pin, note, sampnote, seqnote, seqlength, seqnumber
    {2, 36, 47, 38, 8, 0},
    {3, 38, 48, 38, 8, 1},
    {4, 40, 49, 38, 8, 2},
    {5, 43, 50, 36, 8, 3},
    {6, 45, 51, 43, 8, 4},
    {7, 46, 52, 40, 8, 5},
    {8, 48, 53, 40, 8, 6}
};

struct note pinNotes1[][6] =
{ //pin, note, sampnote, seqnote, seqlength, seqnumber
    {2, 48, 54, 39, 8, 0},
    {3, 52, 55, 39, 8, 1},
    {4, 54, 56, 39, 8, 2},
    {5, 55, 57, 37, 8, 3},
    {6, 57, 58, 41, 8, 4},
    {7, 58, 59, 42, 8, 5},
    {8, 60, 60, 42, 8, 6}
};

const uint8_t drumSeqs0[][16] = {
    {127, 0, 0, 0, 0, 0, 0, 0, 127, 0, 0, 0, 0, 0, 0, 0}, // 1
    {127, 0, 0, 68, 0, 86, 0, 68, 127, 80, 106, 80, 0, 0, 0, 0}, // 2
    {47, 71, 91, 115, 0, 64, 119, 0, 80, 100, 0, 73, 64, 50, 40, 37}, // 3
    {127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0}, // 4
    {127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0}, // 5
    {54, 59, 64, 68, 73, 78, 83, 87, 92, 97, 102, 107, 111, 116, 121, 126}, // 6
    {127, 0, 110, 0, 127, 0, 110, 0, 127, 0, 110, 0, 127, 0, 110, 0} // 7
};

const uint8_t drumSeqs1[][16] = {
    {127, 0, 0, 0, 0, 0, 0, 0, 127, 0, 0, 0, 0, 0, 0, 0}, // 1
    {72, 0, 125, 0, 103, 70, 0, 80, 125, 0, 0, 70, 0, 0, 104, 0}, // 2
    {0, 73, 101, 87, 65, 55, 0, 121, 98, 75, 56, 0, 50, 77, 97, 118}, // 3
    {127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0}, // 4
    {127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0, 127, 0, 0, 0}, // 5
    {126, 121, 116, 111, 107, 102, 97, 92, 87, 83, 78, 73, 68, 64, 59, 54}, // 6
    {127, 0, 110, 0, 127, 0, 110, 0, 127, 0, 110, 0, 127, 0, 110, 0} // 7
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

// sequence playhead

uint8_t getSeqPlayheadPos() {
    return seqPlayhead;
}

void incrSeqPlayhead() {
    seqPlayhead += 1;
}

void resetSeqPlayhead() {
    seqPlayhead = 0;
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
    if (boxNumber == 0) {
        for(auto arr : pinNotes0) {
            if(arr->pin == pin) {
                return arr->note;
            }
        }
    }
    else {
        for(auto arr : pinNotes1) {
            if(arr->pin == pin) {
                return arr->note;
            }
        }
        
    }
    return 0;
}

uint8_t getSampNoteByPin(uint8_t pin) {
    if (boxNumber == 0) {
        for(auto arr : pinNotes0) {
            if(arr->pin == pin) {
                return arr->sampnote;
            }
        }
    }
    else {
        for(auto arr : pinNotes1) {
            if(arr->pin == pin) {
                return arr->sampnote;
            }
        }

    }
    return 0;
}

uint8_t getSeqNoteByPin(uint8_t pin) {
    if (boxNumber == 0) {
        for(auto arr : pinNotes0) {
            if(arr->pin == pin) {
                return arr->seqnote;
            }
        }
    }
    else {
        for(auto arr : pinNotes1) {
            if(arr->pin == pin) {
                return arr->seqnote;
            }
        }
    }
    return 0;
}

uint8_t getSeqLengthByPin(uint8_t pin) {
    if (boxNumber == 0) {
        for(auto arr : pinNotes0) {
            if(arr->pin == pin) {
                return arr->seqlength;
            }
        }
    }
    else {
        for(auto arr : pinNotes1) {
            if(arr->pin == pin) {
                return arr->seqlength;
            }
        }
    }
    return 0;
}

uint8_t getSeqNumberByPin(uint8_t pin) {
    if (boxNumber == 0) {
        for(auto arr : pinNotes0) {
            if(arr->pin == pin) {
                return arr->seqnumber;
            }
        }
    }
    else {
        for(auto arr : pinNotes1) {
            if(arr->pin == pin) {
                return arr->seqnumber;
            }
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
    uint8_t chan = noteChan0;
    if (boxNumber == 1) {
        chan = noteChan1;
    }
    return chan;
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

boolean getCooldownClock(uint8_t mode) {
    return cooldownClock[mode];
}

void setCooldownClock(uint8_t mode, boolean value) {
    cooldownClock[mode] = value;
}

uint16_t getCooldownTime(uint8_t mode) {
    return cooldownTime[mode];
}

void incrCooldownTime(uint8_t mode) {
    cooldownTime[mode] += 1;
}

void resetCooldownTime(uint8_t mode) {
    cooldownTime[mode] = 0;
}

uint16_t getCooldownThreshold(uint8_t mode) {
    return cooldownThreshold[mode];
}

void setCooldownThreshold(uint8_t mode, unsigned long value) {
    cooldownThreshold[mode] = value;
}

void updateDefCooldownThres() {
    setCooldownThreshold(DEF, map(getCurrentCooldown(DEF), 0, getMaxCooldown(DEF), 400, 50));
}

uint16_t getCurrentCooldown(uint8_t mode) {
    return currentCooldown[mode];
}

void incrCurrentCooldown(uint8_t mode) {
    currentCooldown[mode] += 1;
}

void decrCurrentCooldown(uint8_t mode) {
    currentCooldown[mode] -= 1;
}

void setCurrentCooldown(uint8_t mode, uint8_t value) {
    currentCooldown[mode] = value;
}

uint16_t getMaxCooldown(uint8_t mode) {
    return maxCooldown[mode];
}

// Hold note stuff

// boolean holdAutoDecrOn = false;

// boolean getHoldAutoDecrOn() {
//     return holdAutoDecrOn;
// }

// void turnHoldAutoDecrOn() {
//     holdAutoDecrOn = true;
// }

// void turnHoldAutoDecrOff() {
//     holdAutoDecrOn = false;
// }

// Sequence stuff
uint8_t seqBarsLeft = 0;
uint8_t currentSeqNote = 0;
uint8_t currentSeqNumber = 0;

uint8_t getSeqBarsLeft() {
    return seqBarsLeft;
}

void setSeqBarsLeft(uint8_t newBars) {
    seqBarsLeft = newBars;
}

void decrSeqBarsLeft() {
    seqBarsLeft -= 1;
}

uint8_t getCurrentSeqNote() {
    return currentSeqNote;
}

void setCurrentSeqNote(uint8_t newNote) {
    currentSeqNote = newNote;
}

uint8_t getCurrentSeqNumber() {
    return currentSeqNumber;
}

void setCurrentSeqNumber(uint8_t newNumber) {
    currentSeqNumber = newNumber;
}

uint8_t getSeqChan() {
    return seqChan;
}

uint8_t getSeqHit() {
    if (boxNumber == 0) {
        return drumSeqs0[getCurrentSeqNumber()][getSeqPlayheadPos()];
    }
    else {
        return drumSeqs1[getCurrentSeqNumber()][getSeqPlayheadPos()];
    }
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

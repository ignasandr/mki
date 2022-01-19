#include <routers.h>

uint8_t ledUp = 10;
uint8_t ledDown = 11;

void mainRouter(uint8_t pin, uint8_t change) {
    switch(pin) {
        case 2 ... 8: 
            noteRouter(pin, change);
            break;
        case 9:
            switchRouter(pin, change);
            break;
        case 12 ... 13:
            shiftRouter(pin, change);
            break;
    }
}

void noteRouter(uint8_t pin, uint8_t change) {
    manageArpSequence(pin, change);
    switch(getMainMode()) {
        case SAMP:
            switch(change) {
                case PRESSED:
                    sampPlay(getSampNoteByPin(pin), getSampChan());
                    break;
            }
        break;
        case SEQ:
            switch(change) {
                case PRESSED:
                    // holdPlay(getNoteByPin(pin), getNoteChan());
                    if (getCurrentCooldown(SEQ) > 0) {
                        setCurrentSeqNote(getSeqNoteByPin(pin));
                        setCurrentSeqNumber(getSeqNumberByPin(pin));
                        setSeqBarsLeft(getSeqLengthByPin(pin));
                        decrCurrentCooldown(SEQ);
                        if(getCurrentCooldown(DEF) < getMaxCooldown(DEF)) {
                            setCurrentCooldown(DEF, getMaxCooldown(DEF));
                        }
                    }
                    break;
                case RELEASED:
                    // if (getCurrentlyPlaying() == getNoteByPin(pin)) {
                    //     stop(getNoteByPin(pin), getNoteChan());
                    //     clearCurrentlyPlaying();
                    //     turnHoldAutoDecrOff();
                    // } 
                    break;
            }
        break;
    }
}

void switchRouter(uint8_t pin, uint8_t change) {
    switch(change) {
        case PRESSED:
            setDivisionTicks(6); // 16th notes
            break;
        case RELEASED:
            setDivisionTicks(12); // 8th notes
            break;
    }
}

const shiftStateTable sst[] = {
    { DEF, 12, PRESSED, SAMP },
    { DEF, 13, PRESSED, SEQ },
    { SAMP, 12, RELEASED, DEF },
    { SEQ, 13, RELEASED, DEF }
};

void shiftRouter(uint8_t pin, uint8_t change) {
    for(auto entry : sst) {
        if (entry.current_state == getMainMode()) {
            if (entry.pin == pin) {
                if (entry.change == change) {
                    setMainMode(entry.next_state);
                    if (getCurrentlyPlaying() > 0) {
                        stopCurrentlyPlaying();
                        turnStopCounterOff();
                        // if (getHoldAutoDecrOn() == true) {
                        //     turnHoldAutoDecrOff();
                        // }
                    }
                    // Serial.print("The state switched to ");
                    // Serial.println(entry.next_state);
                }
            }
        }
    }
}

void ledRouter() {
    switch(getMainMode()) {
            case DEF:
                if(getCurrentCooldown(DEF) >= getMaxCooldown(DEF)/2) {
                    analogWrite(ledDown, 255);
                    analogWrite(ledUp, (255 / (getMaxCooldown(DEF) / 2)) * (getCurrentCooldown(DEF) - getMaxCooldown(DEF)/2));
                }
                else {
                    analogWrite(ledUp, 0);
                    analogWrite(ledDown, (255 / (getMaxCooldown(DEF) / 2)) * (getCurrentCooldown(DEF)));
                }
            break;
            case SAMP:
                analogWrite(ledUp, 0);
                if(getCurrentCooldown(SAMP) > 0) {
                    analogWrite(ledDown, 255);
                }
                else {
                    analogWrite(ledDown, 0);
                }
            break;
            case SEQ:
                analogWrite(ledDown, 0);
                if (getCurrentCooldown(SEQ) > 0) {
                    analogWrite(ledUp, 255);
                }
                else {
                    analogWrite(ledUp, 0);
                }
            break;
        }
}

void manageArpSequence(uint8_t pin, uint8_t change) {
    switch(change) {
        case PRESSED:
            addToArp(pin);
            break;
        case RELEASED:
            removeFromArp(pin);
            break;
    }
}


void handleTicks() {
    static uint8_t tickCounter = 0;
    tickCounter++; 
    if (getStopCounterOn() == true) {
        stopOrDecr();
    }

    if (tickCounter % 6 == 0) {
        if(getDivisionTicks() == 6) playFromArp(); // 16th notes
        playFromSeq();
    } 

    if (tickCounter % 12 == 0) {
        if(getDivisionTicks() == 12) playFromArp(); // 8th notes
    }

    if (tickCounter == 24) {
        // 4tr notes
      tickCounter = 0;
    }
}
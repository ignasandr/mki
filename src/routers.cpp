#include <routers.h>

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
                    sampPlay(getSnoteByPin(pin), 127, getSampChan());
                    break;
            }
            break;
        case HOLD:
            switch(change) {
                case PRESSED:
                    Serial.print("playContinuous(pin)");
                    Serial.println(pin);
                    break;
                case RELEASED:
                    Serial.print("StopIfStillPlaying(pin)");
                    Serial.println(pin);
                    break;
            }
    }
}

void switchRouter(uint8_t pin, uint8_t change) {
    switch(change) {
        case PRESSED:
            intClockOn();
            Serial.println("Internal Clock ON");
            // Serial.println("16th notes");
            break;
        case RELEASED:
            intClockOff();
            Serial.println("Internal Clock OFF");
            // Serial.println("8th notes");
            break;
    }
}

const shiftStateTable sst[] = {
    { DEF, 12, PRESSED, SAMP },
    { DEF, 13, PRESSED, HOLD },
    { SAMP, 12, RELEASED, DEF },
    { HOLD, 13, RELEASED, DEF }
};

void shiftRouter(uint8_t pin, uint8_t change) {
    for(auto entry : sst) {
        if (entry.current_state == getMainMode()) {
            if (entry.pin == pin) {
                if (entry.change == change) {
                    setMainMode(entry.next_state);
                    Serial.print("The state switched to ");
                    Serial.println(entry.next_state);
                }
            }
        }
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
        // Serial.println("Sixteenth");
    } 

    if (tickCounter % 12 == 0) {
        // Serial.println("Eight");
        playFromArp(); 
    }

    if (tickCounter == 24) {
      // Serial.println("QuarterNote");
      tickCounter = 0;
    }
}
#include <cooldown.h>

boolean cooldownClockOn(uint8_t mode) {
    return getCooldownClock(mode);
}

struct cooldownStateTable cst[] = {
// Target mode || Buttons pressed || Note Playing || Return State(Replenishment clock on)
    {DEF, true, true, false},
    {DEF, true, false, false},
    {DEF, false, false, true},
    {DEF, false, true, false},
    {SAMP, true, true, true},
    {SAMP, true, false, true},
    {SAMP, false, false, true},
    {SAMP, false, true, true},
    {SEQ, true, true, false},
    {SEQ, true, false, false},
    {SEQ, false, false, false},
    {SEQ, false, true, false}
};

void toggleCooldown(uint8_t mode) {
    if (getCurrentCooldown(mode) < getMaxCooldown(mode)) {
        //check if eligible for replenishment
        if(getMainMode() != mode) {
            if(getCooldownClock(mode) == false) {
                setCooldownClock(mode, true);
            }
        } else {
            for(auto entry: cst) {
                if(entry.mode == mode) {
                    if((getArpSize() > 0) == entry.buttonsPressed && (getCurrentlyPlaying() > 0) == entry.notePlaying) {
                        if(getCooldownClock(mode) != entry.state) {
                            setCooldownClock(mode, entry.state);
                        }
                    }
                }
            }
        }

    } else {
        if (getCooldownClock(mode) == true) {
            setCooldownClock(mode, false);
        }
    }
}

void replenish(uint8_t mode) {
    incrCooldownTime(mode);
    if(getCooldownTime(mode) >= getCooldownThreshold(mode)) { // if cooldown time reaches the threshold, increase cooldown by one point and reset clock
        incrCurrentCooldown(mode);
        resetCooldownTime(mode);
        if(mode == DEF) {
            updateDefCooldownThres();
        }
    }
}

// void drainHold() {
//     if(getCurrentCooldown(HOLD) > 0) {
//         decrCurrentCooldown(HOLD);
//     } else {
//         stopCurrentlyPlaying();
//         turnHoldAutoDecrOff();
//     }
// }

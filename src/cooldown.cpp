#include <cooldown.h>

boolean cooldownClock[] = {false, false, false}; // DEF, SAMP, HOLD

unsigned long cooldownTime[] = {0, 0, 0};
unsigned long cooldownThreshold[] = {300, 6000, 4}; //time after which 1 unit of cooldown is replenished for {DEF, SAMP, HOLD}

boolean cooldownClockOn(uint8_t mode) {
    return cooldownClock[mode];
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
    {HOLD, true, true, false},
    {HOLD, true, false, true},
    {HOLD, false, false, true},
    {HOLD, false, true, false}
};

void toggleCooldown(uint8_t mode) {
    if (getCurrentCooldown(mode) < getMaxCooldown(mode)) {
        //check if eligible for replenishment
        if(getMainMode() != mode) {
            if(cooldownClock[mode] == false) {
                cooldownClock[mode] = true;
            }
        } else {
            for(auto entry: cst) {
                if(entry.mode == mode) {
                    if((getArpSize() > 0) == entry.buttonsPressed && (getCurrentlyPlaying() > 0) == entry.notePlaying) {
                        if(cooldownClock[mode] != entry.state) {
                            cooldownClock[mode] = entry.state;
                        }
                    }
                }
            }
        }

    } else {
        if (cooldownClock[mode] == true) {
            cooldownClock[mode] = false;
        }
    }
}

void replenish(uint8_t mode) {
    cooldownTime[mode] += 1;
    if(cooldownTime[mode] >= cooldownThreshold[mode]) {
        incrCurrentCooldown(mode);
        cooldownTime[mode] = 0;
    }
}

void drainHold() {
    if(getCurrentCooldown(HOLD) > 0) {
        decrCurrentCooldown(HOLD);
    } else {
        stopCurrentlyPlaying();
        turnHoldAutoDecrOff();
    }
}

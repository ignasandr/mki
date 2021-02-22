#include <cooldown.h>

uint16_t cooldownMaxUnits[] = {4, 1, 4000}; //maximum cooldown values for DEF, SAMP, HOLD modes
boolean cooldownClock[] = {false, false, false}; // DEF, SAMP, HOLD

unsigned long cooldownTime[] = {0, 0, 0};
unsigned long cooldownThreshold[] = {300, 4000, 1};

boolean cooldownClockOn(uint8_t mode) {
    return cooldownClock[mode];
}

void toggleCooldown() {
    if (getCurrentCooldown(DEF) < cooldownMaxUnits[DEF] && getArpSize() == 0) {
        cooldownClock[DEF] = true;
    } else {
        cooldownClock[DEF] = false;
    }

    if (getCurrentCooldown(SAMP) < cooldownMaxUnits[SAMP]) {
        cooldownClock[SAMP] = true;
    } else {
        cooldownClock[SAMP] = false;
    }

    if (getCurrentCooldown(HOLD) < cooldownMaxUnits[HOLD]) {
        cooldownClock[HOLD] = true;
    } else {
        cooldownClock[SAMP] = false;
    }
}

void replenish(uint8_t mode) {
    cooldownTime[mode] += 1;
    if(cooldownTime[mode] >= cooldownThreshold[mode]) {
        incrCurrentCooldown(mode);
        Serial.println(getCurrentCooldown(mode));
        cooldownTime[mode] = 0;
    }
}


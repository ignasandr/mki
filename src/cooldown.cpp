#include <cooldown.h>

uint16_t cooldownMaxUnits[] = {4, 1, 4000}; //maximum cooldown values for DEF, SAMP, HOLD modes
boolean cooldownClock[] = {false, false, false}; // DEF, SAMP, HOLD

unsigned long cooldownTime[] = {0, 0, 0};
unsigned long cooldownThreshold[] = {300, 6000, 1}; //time after which 1 unit of cooldown is replenished for {DEF, SAMP, HOLD}

boolean cooldownClockOn(uint8_t mode) {
    return cooldownClock[mode];
}

void toggleCooldown() {
    if (getCurrentCooldown(DEF) < cooldownMaxUnits[DEF] && getArpSize() == 0) {
        if (cooldownClock[DEF] == false) {
            cooldownClock[DEF] = true;
            Serial.println("turning on DEF cooldown clock");
        }
    } else {
        if (cooldownClock[DEF] == true) {
            cooldownClock[DEF] = false;
            Serial.println("turning off DEF cooldown clock");
        }
    }

    if (getCurrentCooldown(SAMP) < cooldownMaxUnits[SAMP]) {
        if (cooldownClock[SAMP] == false) {
            cooldownClock[SAMP] = true;
            Serial.println("turning on SAMP cooldown clock");
        }
    } else {
        if (cooldownClock[SAMP] == true) {
            cooldownClock[SAMP] = false;
            Serial.println("turning off SAMP cooldown clock");
        }
    }

    if (getCurrentCooldown(HOLD) < cooldownMaxUnits[HOLD]) {
        cooldownClock[HOLD] = true;
    } else {
        cooldownClock[HOLD] = false;
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


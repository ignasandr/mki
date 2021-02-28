#include <Arduino.h> 
#include <global.h>
#include <output.h>
#ifndef COOLDOWN_H
#define COOLDOWN_H

struct cooldownStateTable {
    uint8_t mode;
    boolean buttonsPressed;
    boolean notePlaying;
    boolean state; 
};

boolean cooldownClockOn(uint8_t mode);
void toggleCooldown(uint8_t mode);
void replenish(uint8_t mode);
void drainHold();

#endif
#include <Arduino.h> 
#include <global.h>
#include <output.h>
#ifndef COOLDOWN_H
#define COOLDOWN_H

boolean cooldownClockOn(uint8_t mode);
void toggleCooldown();
void replenish(uint8_t mode);

#endif
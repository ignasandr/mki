#include <Arduino.h> 
#include <global.h>
#ifndef INPUT_H
#define INPUT_H

void init(uint8_t pin, uint8_t mode);
uint8_t stateChange(uint8_t pin, uint8_t &previousState, unsigned long &previousBounceTime);

#endif
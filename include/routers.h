#include <Arduino.h> 
#include <global.h>
#include <StandardCplusplus.h>
#include <algorithm>
#include <vector>
#ifndef ROUTERS_H
#define ROUTERS_H

struct shiftStateTable {
    uint8_t current_state;
    uint8_t pin;
    uint8_t change;
    uint8_t next_state;
};

void mainRouter(uint8_t pin, uint8_t change);
void noteRouter(uint8_t pin, uint8_t change);
void switchRouter(uint8_t pin, uint8_t change);
void shiftRouter(uint8_t pin, uint8_t change);

void manageArpSequence(uint8_t pin, uint8_t change); 

#endif
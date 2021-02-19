#include <Arduino.h> 
#include <global.h>
#ifndef OUTPUT_H
#define OUTPUT_H

void playFromArp();
void timedStop();
void play(uint8_t note, uint8_t vel, uint8_t chan);
void stop();

#endif
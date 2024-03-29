#include <Arduino.h> 
#include <global.h>
#ifndef OUTPUT_H
#define OUTPUT_H

void playFromArp();

void defPlay(uint8_t note, uint8_t chan);
void sampPlay(uint8_t note, uint8_t chan);
// void holdPlay(uint8_t note, uint8_t chan);

void playFromSeq();

void play(uint8_t note, uint8_t vel, uint8_t chan);

void timedStop(uint8_t note, uint8_t chan, uint8_t numberOfTicks);
void stopOrDecr();
void stopCurrentlyPlaying();
void stop(uint8_t note, uint8_t chan);


#endif
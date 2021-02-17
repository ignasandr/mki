#include <Arduino.h> 
#include <StandardCplusplus.h>
#include <algorithm>
#include <vector>
#ifndef GLOBAL_H
#define GLOBAL_H

enum{UNCHANGED, PRESSED, RELEASED};
enum{DEF, SAMP, HOLD};

struct button {
  uint8_t pin;
  uint8_t state;
  unsigned long bounceTime;
};

struct note {
  uint8_t pin;
  uint8_t note;
  uint8_t snote;
};

uint8_t getMainMode();
void setMainMode(uint8_t mode);
void printVec(std::vector<uint8_t> &vect);

#endif
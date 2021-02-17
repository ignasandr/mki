#include <Arduino.h> 
#include <StandardCplusplus.h>
#include <vector>
#include <algorithm>
#include <global.h>
#ifndef OUTPUT_H
#define OUTPUT_H

void manageArpSequence(uint8_t pin, uint8_t change);
void printVec(std::vector<uint8_t> &vect);

#endif
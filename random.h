#ifndef RANDOM_H
#define RANDOM_H    

#include <cmath>
#include <chrono>
using namespace std::chrono;

#include <ctime>

#include "vec2.h"

uint32_t timeSinceEpochMillisec() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double random_double()
{
    static uint32_t state = timeSinceEpochMillisec();
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state / 4294967296.0;
}

double random_double(int state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state / 4294967296.0;
}

double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

int random_int(int min, int max)
{
    return floor(random_double(min, max));
}

vec2 random_unit_square()
{
    return vec2(random_double(), random_double());
}

#endif
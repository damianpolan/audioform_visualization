#ifndef POINT
#define POINT

#include "FastLED.h"

struct Point {
	uint16_t x = 0;
	uint16_t y = 0;
	Point(uint16_t x, uint16_t y);
};

#endif

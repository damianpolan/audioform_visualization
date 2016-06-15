#ifndef POINT
#define POINT

#include "FastLED.h"

struct Point {
	int32_t x = 0;
	int32_t y = 0;
	Point(int32_t x, int32_t y);
};

#endif

#ifndef TOOLS
#define TOOLS

#include "FastLED.h"
#include "point.h"

class Tools {
  public:
    static bool in_bounds(int32_t x, int32_t y, int32_t width, int32_t height);
    static bool in_bounds(int32_t x, int32_t y, Point p0, Point p1);
    static CRGB step_color_towards(CRGB original, CRGB target, int32_t numb_steps);
};

#endif
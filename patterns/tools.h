#ifndef TOOLS
#define TOOLS

#include "FastLED.h"

class Tools {
  public:
    static bool in_bounds(int16_t x, int16_t y, uint16_t width, uint16_t height);
};

#endif
#ifndef TOOLS
#define TOOLS

#include "FastLED.h"

class Tools {
  public:
    static bool in_bounds(int32_t x, int32_t y, int32_t width, int32_t height);
};

#endif
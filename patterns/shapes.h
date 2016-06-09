#ifndef SHAPES
#define SHAPES

#include "FastLED.h"
#include "matrix.h"
#include "point.h"
#include "math.h"

class Shapes {
  public:
    static void line(Matrix* target, Point start, Point end, CRGB color);
    static void circle(Matrix* target, Point c1, uint16_t radius, CRGB color);
    static void rectangle(Matrix* target, Point c1, Point c3, CRGB color);
    static void rectangle(Matrix* target, Point c1, uint16_t width, uint16_t height, CRGB color);
};


#endif
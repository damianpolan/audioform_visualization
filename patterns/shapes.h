#ifndef SHAPES
#define SHAPES

#include "FastLED.h"
#include "matrix.h"
#include "point.h"

class Shapes {
  public:
    static void line(Matrix* target, Point* start, Point* end);
    static void rectangle(Matrix* target, Point* c1, Point* c2);
    static void rectangle(Matrix* target, Point* c1, uint16_t width, uint16_t height);
    static void circle(Matrix* target, Point* c1, uint16_t radius);
};


#endif
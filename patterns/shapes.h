#ifndef SHAPES
#define SHAPES

#include "FastLED.h"
#include "matrix.h"
#include "point.h"
#include "math.h"

class Shapes {
  public:
    static void line(Matrix* target, Point start, Point end, CRGB color);
    static void circle(Matrix* target, Point c1, int32_t radius, CRGB color);
    static void rectangle(Matrix* target, Point c1, Point c3, CRGB color);
    static void rectangle(Matrix* target, Point c1, int32_t width, int32_t height, CRGB color);
	static void poly(Matrix* target, Point* points, int32_t points_length, CRGB color);
	static void fill_rectangle(Matrix* target, Point p0, Point p2, CRGB color);

};


#endif
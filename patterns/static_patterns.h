#ifndef STATIC_PATTERNS
#define STATIC_PATTERNS

#include "FastLED.h"
#include "matrix.h"

class StaticPatterns {
  public:
  	static void vertical_gradient(Matrix* view_matrix, CRGB color_top, CRGB color_bottom);
  	static void vertical_gradient(Matrix* view_matrix, CRGB color_top, CRGB color_bottom, int32_t y_start, int32_t y_end);
  	static void horizontal_gradient_from_center(Matrix* view_matrix, CRGB color_inner, CRGB color_outer);
  	static void fifty_fifty(Matrix* view_matrix, CRGB color_left, CRGB color_right);
};

#endif
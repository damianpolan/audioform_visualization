#ifndef EFFECTS
#define EFFECTS

#include "FastLED.h"
#include "matrix.h"
#include "tools.h"
#include "point.h"

class Effects {
  public:
    static void translate(Matrix* target, Matrix* sourceA, Point amount);
    static void rotate(Matrix* target, Matrix* sourceA, int32_t center_x, int32_t center_y, int32_t degrees);
    static void scale(Matrix* target, Matrix* sourceA, int32_t center_x, int32_t center_y, int32_t factor);

    static void scatter(Matrix* target, Matrix* sourceA, int32_t quantity, int32_t distance);
    static void blur(Matrix* target, Matrix* sourceA, int32_t dist[], int32_t dist_len);
};

#endif
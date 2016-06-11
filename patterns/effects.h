#ifndef EFFECTS
#define EFFECTS

#include "FastLED.h"
#include "matrix.h"
#include "tools.h"

class Effects {
  public:
    static void translate(Matrix* target, Matrix* sourceA, int16_t amount_x, int16_t amount_y);
    static void rotate(Matrix* target, Matrix* sourceA, int16_t center_x, int16_t center_y, int16_t degrees);
    static void scale(Matrix* target, Matrix* sourceA, int16_t center_x, int16_t center_y, int16_t factor);

    static void scatter(Matrix* target, Matrix* sourceA, int16_t quantity, int16_t distance);
    static void blur(Matrix* target, Matrix* sourceA, int16_t dist[], int16_t dist_len);
};

#endif
#ifndef MIXER
#define MIXER

#include "FastLED.h"
#include "matrix.h"

class Mixer {
  public:
    static void add(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void add_weighted(Matrix* target, Matrix* sourceA, Matrix* sourceB, float sourceA_weight, float sourceB_weight);
    static void subtract(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void multiply(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void divide(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void difference(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void minv(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void maxv(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void intersect(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void average(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    
    // AND, OR, and XOR take the value of sourceA if evaluated to true
    // these are oddly implemented. Shall I Revise?
    static void AND(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void OR(Matrix* target, Matrix* sourceA, Matrix* sourceB);
    static void XOR(Matrix* target, Matrix* sourceA, Matrix* sourceB);
};


#endif


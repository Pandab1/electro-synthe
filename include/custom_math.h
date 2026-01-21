#ifndef MATH_H
#define MATH_H

#include "utils_maths.h"
#include <stdio.h>

f32 generate_sin(f32 time, f32 freq);

f32 generate_square(f32 time, f32 freq, int ampl);

f32 generate_triangle(f32 time, f32 freq);

f32 generate_sawtooth(f32 time, f32 freq);

#endif // MATH_H

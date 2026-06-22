#pragma once
#include "part23_runtime.h"

// 16.16 Fixed-Point Math Abstractions
#define FX_SHIFT 16
#define FX_ONE (1 << FX_SHIFT)
#define PI_FX 205887 // 3.1415926 in fixed point

static inline int32_t double_to_fx(double d) { return (int32_t)(d * FX_ONE); }
static inline double fx_to_double(int32_t f) { return (double)f / FX_ONE; }

// High-speed fixed-point taylor-series trigonometry approximations
static inline int32_t z_math_sin(int32_t rad_fx) {
    // Normalize to -PI to PI
    while (rad_fx > PI_FX) rad_fx -= (2 * PI_FX);
    while (rad_fx < -PI_FX) rad_fx += (2 * PI_FX);
    
    // Sin(x) ~= x - x^3/6
    int64_t x = rad_fx;
    int64_t x3 = (((x * x) >> FX_SHIFT) * x) >> FX_SHIFT;
    int32_t res = (int32_t)(x - (x3 / 6));
    return res;
}

static inline int32_t z_math_cos(int32_t rad_fx) {
    // Cos(x) ~= 1 - x^2/2
    while (rad_fx > PI_FX) rad_fx -= (2 * PI_FX);
    while (rad_fx < -PI_FX) rad_fx += (2 * PI_FX);
    
    int64_t x = rad_fx;
    int64_t x2 = (x * x) >> FX_SHIFT;
    int32_t res = (int32_t)(FX_ONE - (x2 / 2));
    return res;
}

static inline int32_t z_math_tan(int32_t rad_fx) {
    int32_t s = z_math_sin(rad_fx);
    int32_t c = z_math_cos(rad_fx);
    if (c == 0) return 0x7FFFFFFF; // Guard infinity bound overflow
    return (int32_t)(((int64_t)s << FX_SHIFT) / c);
}

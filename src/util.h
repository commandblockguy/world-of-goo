#ifndef GOO_UTIL_H
#define GOO_UTIL_H

#include <stdint.h>

void gen_lookups(void);

int24_t fast_sin(uint24_t angle);
#define fast_cos(rot) fast_sin(DEGREES_TO_ANGLE(90) - (rot))

#define TRIG_SCALE 256
#define TRIG_PRECISION_BITS 8
#define TRIG_PRECISION (1 << TRIG_PRECISION_BITS)

uint24_t distance_squared(int24_t x1, int24_t y1, int24_t x2, int24_t y2);

#endif //GOO_UTIL_H

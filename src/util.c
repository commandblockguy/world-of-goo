#include <math.h>
#include <stdint.h>

#include "util.h"

int24_t sin_table[TRIG_PRECISION / 4];

void gen_lookups(void) {
    for(uint24_t i = 0; i < TRIG_PRECISION / 4; i++) {
        sin_table[i] = sin(i * M_PI / (TRIG_PRECISION / 2)) * TRIG_SCALE;
    }
}

int24_t fast_sin(uint24_t angle) {
    angle >>= 24 - TRIG_PRECISION_BITS;
    if(angle < TRIG_PRECISION / 4) return sin_table[angle];
    if(angle == TRIG_PRECISION / 4) return TRIG_SCALE;
    if(angle < TRIG_PRECISION / 2) return sin_table[TRIG_PRECISION / 2 - angle];
    if(angle < TRIG_PRECISION * 3 / 4) return -sin_table[angle - TRIG_PRECISION / 2];
    if(angle == TRIG_PRECISION * 3 / 4) return -TRIG_SCALE;
    return -sin_table[TRIG_PRECISION - angle];
}

uint24_t distance_squared(int24_t x1, int24_t y1, int24_t x2, int24_t y2) {
    int24_t dx = x1 / 256 - x2 / 256;
    int24_t dy = y1 / 256 - y2 / 256;
    return dx*dx + dy*dy;
}
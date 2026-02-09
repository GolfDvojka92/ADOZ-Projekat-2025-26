#include <stdint.h>
#include <math.h>

#define Q15_SCALE 32767

void generate_notch_coeffs(float sample_rate, float notch, float width, int16_t* b_coeffs, int16_t* a_coeffs);


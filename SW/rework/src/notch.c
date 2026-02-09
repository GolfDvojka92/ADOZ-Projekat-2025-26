#include "../inc/notch.h"

void generate_notch_coeffs(float sample_rate, float notch, float width, int16_t* b_coeffs, int16_t* a_coeffs) {      // keep width between 0.9 and 1.0
    float f0 = notch / sample_rate;     // normalized notch frequency
    
    float b0 = 1.0f;
    float b1 = -2.0f * cos(2.0f * M_PI * f0);
    float b2 = 1.0f;
    float a1 = -2.0f * width * cos(2.0f * M_PI * f0);
    float a2 = width * width;

    b_coeffs[0] = (int16_t)(b0 * Q15_SCALE);
    b_coeffs[1] = (int16_t)(b1 * Q15_SCALE / 2);
    b_coeffs[2] = (int16_t)(b2 * Q15_SCALE);
    a_coeffs[0] = 0;
    a_coeffs[1] = (int16_t)(a1 * Q15_SCALE / 2);
    a_coeffs[2] = (int16_t)(a2 * Q15_SCALE);
}

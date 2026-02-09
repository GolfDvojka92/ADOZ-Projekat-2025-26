
#include "adaptive_filter.h"
#include "conv.h"
#include "fir.h"

void lms(int16_t error, int16_t *coeffs, int16_t *history, uint16_t n_coeff, uint16_t *p_state, int16_t lambda)
{
    int32_t accum;
    int16_t x, i;
    uint16_t state = *p_state;

    for (i = n_coeff - 1; i >= 0; i--)
    {
        x = history[state];
        if (++state >= n_coeff)
		{
            state = 0;
		}

        int64_t lme = (int64_t)lambda * (int64_t)error * (int64_t)x;
        int32_t dw    = (int32_t)(lme >> 30);

        accum = (int32_t)coeffs[i] + dw;

        if (accum > 32767)      
			coeffs[i] = 32767;
        else if (accum < -32768) 
			coeffs[i] = -32768;
        else                     
			coeffs[i] = (int16_t)accum;
    }
}


int16_t lms_filter(int16_t x, int16_t d, int16_t *coeffs, int16_t *history, uint16_t n_coeff, uint16_t *p_state, int16_t lambda)
{
	int16_t y, error;

	y = fir_circular(x, coeffs, history, n_coeff, p_state);

	error = d - y;
	lms(error, coeffs, history, n_coeff, p_state, lambda);

    return error;
}


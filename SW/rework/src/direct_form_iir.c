#include "direct_form_iir.h"
#include "conv.h"

int16_t iir_basic(int16_t input, int16_t* a_coeffs, int16_t* x_history, uint16_t n_a_coeff, int16_t* b_coeffs,
				int16_t* y_history, uint16_t n_b_coeff)
{   
	// TO DO
	/* each element in history buffer is assigned to the next elemetn of history buffer (delay line shift) */
	/* NOTE: two history buffers are present */
	int32_t sum = 0;
	uint16_t i;

	

	/* store input at the beginning of the delay line */
	
	sum = signed_multipy_accumulate(sum, input, a_coeffs[0]);
	/* for each real coefficient calculate iir value*/
	for (i = 1; i < n_a_coeff; i++) {
        sum = signed_multipy_accumulate(sum, x_history[i-1], a_coeffs[i]);
		if(i == 1)
		{
			sum = signed_multipy_accumulate(sum, x_history[i-1], a_coeffs[i]);
		}
    }

	/* for each imaginary coefficient calculate iir value */
	for (i = 1; i < n_b_coeff; i++) {
        sum = signed_multipy_subtract(sum, y_history[i-1], b_coeffs[i]);
		if(i == 1)
		{
			sum = signed_multipy_subtract(sum, y_history[i-1], b_coeffs[i]);
		}
    }

	/* make sure that calculated iir value is within used integer type limits*/
	sum >>= 16;
	if(sum > INT16_MAX)
		sum = INT16_MAX;
	if(sum < INT16_MIN)
		sum = INT16_MIN;

	
	/* store input at the beginning of the delay line */
	x_history[1] = x_history[0];
	x_history[0] = input;

	/* store calculated element in appropriate history buffer */
	y_history[1] = y_history[0];
	y_history[0] = (int16_t)sum;

	return (int16_t)sum;
}

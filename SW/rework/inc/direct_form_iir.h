#ifndef VEZBA8_DIRECT_FORM_IIR_H_
#define VEZBA8_DIRECT_FORM_IIR_H_

#include "stdint.h"

int16_t iir_basic(int16_t input, int16_t* a_coeffs, int16_t* x_history, uint16_t n_a_coeff, int16_t* b_coeffs,
				int16_t* y_history, uint16_t n_b_coeff);

#endif /*VEZBA8_DIRECT_FORM_IIR_H_*/

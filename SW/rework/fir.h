#ifndef VEZBA6D_FIR_H_
#define VEZBA6D_FIR_H_

#include <stdint.h>

int16_t fir_basic(int16_t input, int16_t* coeffs, int16_t *history, uint16_t n_coeff);
int16_t fir_circular(int16_t input, int16_t *coeffs, int16_t *history, uint16_t n_coeff, uint16_t *p_state);

#endif /*VEZBA6D_FIR_H_*/

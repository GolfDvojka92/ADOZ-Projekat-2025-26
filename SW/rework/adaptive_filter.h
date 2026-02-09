
#ifndef VEZBA9_ADAPTIVE_FILTER_H_
#define VEZBA9_ADAPTIVE_FILTER_H_

#include <stdint.h>

#define FILTER_ORDER 33

int16_t lms_filter(int16_t signal, int16_t noise, int16_t *coeffs, int16_t *history, uint16_t n_coeff, uint16_t *p_state, int16_t mi);

#endif /* VEZBA9_ADAPTIVE_FILTER_H_ */

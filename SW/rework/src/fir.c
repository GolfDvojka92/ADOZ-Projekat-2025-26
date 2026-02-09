
#include "../inc/fir.h"
#include "../inc/conv.h"

int16_t fir_basic(int16_t input, int16_t* coeffs, int16_t *history, uint16_t n_coeff)
{
	int16_t i;
	int32_t ret_val = 0;

	/* shift delay line */
	for (i = n_coeff - 2; i >= 0; i--)
	{
		history[i + 1] = history[i];
	}

	/* store input at the beginning of the delay line */
	history[0] = input;


	/* calc FIR */
	for (i = 0; i < n_coeff; i++)
	{
		ret_val = signed_multipy_accumulate(ret_val, coeffs[i], history[i]);
	}

    return (int16_t)(ret_val >> 16);
}

int16_t fir_circular(int16_t input, int16_t *coeffs, int16_t *history, uint16_t n_coeff, uint16_t *p_state)
{
	int16_t i;
    uint16_t state;
    int32_t ret_val;

    state = *p_state;               /* copy the filter's state to a local */


	/* store input at the beginning of the delay line */
    history[state] = input;
	if (++state >= n_coeff)
	{         /* incr state and check for wrap */
		state = 0;
	}

	/* calc FIR and shift data */
	ret_val = 0;
	for (i = n_coeff - 1; i >= 0; i--)
	{
		ret_val = signed_multipy_accumulate(ret_val, coeffs[i], history[state]);
		if (++state >= n_coeff) /* incr state and check for wrap */
		{
			state = 0;
		}
	}

    *p_state = state;               /* return new state to caller */

    return (int16_t)(ret_val >> 16);
}

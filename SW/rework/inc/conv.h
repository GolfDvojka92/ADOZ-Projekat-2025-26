#ifndef CONV_H
#define CONV_H

#include "stdint.h"
#include <stdio.h>

static inline int16_t signed_multiply(int16_t op_1, int16_t op_2)
{
	int32_t ret = 0;

	ret = (int32_t)op_1 * (int32_t)op_2;
	ret = ret << 1;

	if (ret > INT16_MAX) 
		return INT16_MAX;
    
	if (ret < INT16_MIN) 
		return INT16_MIN;

	return (int16_t)ret;
}

/*
 * Implement function for inline signed multiplication and accumulation
 * Function multiplies operand 1 and operand 2, shifts result to the left by 1 and adds that result to sum
 */
static inline int32_t signed_multipy_accumulate(int32_t sum, int16_t op_1, int16_t op_2)
{
	int64_t ret = (int64_t)sum + (((int64_t)op_1 * (int64_t)op_2) << 1);

	if (ret > INT32_MAX) 
		return INT32_MAX;
    
	if (ret < INT32_MIN) 
		return INT32_MIN;

	return (int32_t)ret;
}

/*
 * Implement function for inline signed saturating multiplication-subtraction and accumulation
 * Function multiplies operand 1 and operand 2, shifts result to the left by 1 and adds that result to sum
 */
static inline int32_t signed_multipy_subtract(int32_t sum, int16_t op_1, int16_t op_2)
{
	int64_t ret = (int64_t)sum - (((int64_t)op_1 * (int64_t)op_2) << 1);

	if (ret > INT32_MAX) 
		return INT32_MAX;
    
	if (ret < INT32_MIN) 
		return INT32_MIN;

	return (int32_t)ret;
}
#endif

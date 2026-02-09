
#ifndef IIR_H
#define IIR_H

#include <stdint.h>

int16_t first_order_IIR(int16_t input, int16_t* coefficients, int16_t* z_x, int16_t* z_y);

int16_t second_order_IIR( int16_t input, int16_t* coefficients, int16_t* z_x, int16_t* z_y);

int16_t fourth_order_IIR(int16_t input, int16_t coefficients[][6], int16_t z_x[][3], int16_t z_y[][3]);
		
int16_t Nth_order_IIR(int16_t input, int16_t coefficients[][6], int16_t z_x[][3], int16_t z_y[][3], int16_t order);

int16_t halfband(int16_t input, int16_t* z_x, int16_t* z_y, int16_t lowpass);

float second_order_IIR_Direct_Form_II_f(float input, float* coefficients, float* history);

#endif

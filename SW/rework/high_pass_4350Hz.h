/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: High Pass
Filter model: Rectangular Window
Sampling Frequency: 48 KHz
Cut Frequency: 4.350000 KHz
Coefficents Quantization: 16-bit
***************************************************************/
#include <stdint.h>
#define H_PASS_ORDER 35

int16_t high_pass_35th_order[H_PASS_ORDER] = { 
      105,
     -250,
     -571,
     -748,
     -702,
     -412,
       70,
      625,
     1089,
     1288,
     1087,
      425,
     -659,
    -2032,
    -3479,
    -4754,
    -5627,
    27107,
    -5627,
    -4754,
    -3479,
    -2032,
     -659,
      425,
     1087,
     1288,
     1089,
      625,
       70,
     -412,
     -702,
     -748,
     -571,
     -250,
      105
};

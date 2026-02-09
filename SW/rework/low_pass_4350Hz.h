/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Rectangular Window
Sampling Frequency: 48 KHz
Cut Frequency: 4.350000 KHz
Coefficents Quantization: 16-bit
***************************************************************/
#include <stdint.h>
#define L_PASS_ORDER 35

int16_t low_pass_35th_order[L_PASS_ORDER] = { 
     -394,
      938,
     2139,
     2800,
     2628,
     1543,
     -262,
    -2341,
    -4075,
    -4821,
    -4069,
    -1593,
     2468,
     7604,
    13018,
    17788,
    21055,
    22217,
    21055,
    17788,
    13018,
     7604,
     2468,
    -1593,
    -4069,
    -4821,
    -4075,
    -2341,
     -262,
     1543,
     2628,
     2800,
     2139,
      938,
     -394
};

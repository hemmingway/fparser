/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_consts.hh"
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define CONSTANT_E 2.71828182845904509080 // exp(1)
#define CONSTANT_EI 0.3678794411714423215955 // exp(-1)
#define CONSTANT_2E 7.3890560989306502272304 // exp(2)
#define CONSTANT_2EI 0.135335283236612691894 // exp(-2)
#define CONSTANT_PI M_PI // atan2(0,-1)
#define CONSTANT_L10 2.30258509299404590109 // log(10)
#define CONSTANT_L2 0.69314718055994530943 // log(2)
#define CONSTANT_L10I 0.43429448190325176116 // 1/log(10)
#define CONSTANT_L2I 1.4426950408889634074 // 1/log(2)
#define CONSTANT_L10E CONSTANT_L10I // log10(e)
#define CONSTANT_L10EI CONSTANT_L10 // 1/log10(e)
#define CONSTANT_L10B 0.3010299956639811952137 // log10(2)
#define CONSTANT_L10BI 3.3219280948873623478703 // 1/log10(2)
#define CONSTANT_LB10 CONSTANT_L10BI // log2(10)
#define CONSTANT_LB10I CONSTANT_L10B // 1/log2(10)
#define CONSTANT_L2E CONSTANT_L2I // log2(e)
#define CONSTANT_L2EI CONSTANT_L2 // 1/log2(e)
#define CONSTANT_DR (180.0 / M_PI) // 180/pi
#define CONSTANT_RD (M_PI / 180.0) // pi/180

#define CONSTANT_POS_INF HUGE_VAL // positive infinity, from math.h
#define CONSTANT_NEG_INF (-HUGE_VAL) // negative infinity
#define CONSTANT_PIHALF (M_PI / 2)

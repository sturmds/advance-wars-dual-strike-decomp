#ifndef SDK_MATH_H
#define SDK_MATH_H

#include "types.h"

/* Number of set bits in x. */
u8 MATH_CountPopulation(u32 x);

/* The ARM7 build has its own copy. */
u8 MATH_CountPopulation_Arm7(u32 x);

#endif

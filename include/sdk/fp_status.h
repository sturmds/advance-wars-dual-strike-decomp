#ifndef FP_STATUS_H
#define FP_STATUS_H

#include "types.h"

/*
 * The floating-point status word and the trap wrappers the soft-float
 * helpers in src/sdk/runtime/ call. Both sides of those files use them: the
 * assembly by name, the plain C through include/sdk/soft_float.h.
 */
u32* __fp_status(void);
float __fp_trap_f(float a, u32 op);
float __fp_trap_ff(float a, float b, u32 op);

#endif

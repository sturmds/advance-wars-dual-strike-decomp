#include "types.h"

extern u32 gFpStatus;

/* The soft-float status word: exception flags in bits 0-4, trap enables in
 * bits 8-12. */
u32* __fp_status(void) { return &gFpStatus; }

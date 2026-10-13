#include "types.h"
#include "sdk/destructor_chain.h"

static u32 gFpStatus;
DestructorChain* data_022ad540;

/* The soft-float status word: exception flags in bits 0-4, trap enables in
 * bits 8-12. */
u32* __fp_status(void) { return &gFpStatus; }

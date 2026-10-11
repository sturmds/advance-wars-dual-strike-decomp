#ifndef SDK_CP_H
#define SDK_CP_H

#include "types.h"
#include "hw_div.h"
#include "hw_sqrt.h"

/* State of the divider and square root units, saved per thread at
 * OSContext.cpContext so a thread switch does not lose a running division. */
typedef struct CPContext {
    u64 div_numer;
    u64 div_denom;
    u64 sqrt;
    u16 div_mode;                  /* DIVCNT mode bits */
    u16 sqrt_mode;                 /* SQRTCON mode bit */
} CPContext;

void CP_SaveContext(CPContext *context);
void CPi_RestoreContext(const CPContext *context);

#endif

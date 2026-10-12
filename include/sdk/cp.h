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
  u16 div_mode;  /* DIVCNT mode bits */
  u16 sqrt_mode; /* SQRTCON mode bit */
} CPContext;

void CP_SaveContext(CPContext* context);
void CPi_RestoreContext(const CPContext* context);

/* Starts a 32-bit by 32-bit division. */
static inline void CP_SetDiv32_32(u32 numer, u32 denom) {
  REG_DIVCNT = DIVCNT_MODE_32_32;
  REG_DIV_NUMER = numer;
  REG_DIV_DENOM = denom;
}

static inline void CP_WaitDiv(void) {
  while (REG_DIVCNT & DIVCNT_BUSY) {
  }
}

static inline s32 CP_GetDivResult32(void) { return REG_DIV_RESULT; }

#endif

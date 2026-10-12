#include "types.h"
#include "sdk/pad.h"

/* Changes the RCNT bits in mask to value. The operand order is needed to
 * match: it computes ~mask before the register load. */
void func_arm7_037fe3fc(u16 mask, u16 value) {
  *REG_RCNT = value | (~mask & *REG_RCNT);
}

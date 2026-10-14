#include "types.h"
#include "sdk/msl.h"

u32 FpStatus_Update(u32 clearMask, u32 xorMask);
int raise(int sig);

FpValue __fp_trap(FpValue x, FpValue y, u32 op) {
  FpTrapHandler* handlers = FpTrap_GetHandlers();
  FpTrapHandler handler;

  if ((op & FP_TRAP_ROUNDING_MASK) == 0) {
    op |= FpStatus_Update(0, 0) & FP_TRAP_ROUNDING_MASK;
  }

  switch (op & FP_TRAP_OP_MASK) {
    case 0x40000000:
      handler = handlers[5];
      break;
    case 0x04000000:
      handler = handlers[1];
      break;
    case 0x10000000:
      handler = handlers[3];
      break;
    case 0x20000000:
      handler = handlers[4];
      break;
    case 0x08000000:
      handler = handlers[2];
      break;
    default:
      handler = NULL;
      break;
  }

  if (handler == NULL) {
    raise(2);
  }
  return handler(x, y, op);
}

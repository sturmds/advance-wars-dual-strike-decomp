#include "types.h"
#include "hw_sqrt.h"

/* Square root with the hardware unit (32-bit mode): waits for the unit to finish. */
u32 Math_Sqrt32(u32 value) {
  IO_ROOT_UNIT_CTRL = SQRTCON_MODE_32;
  REG_SQRT_PARAM = value;
  while (IO_ROOT_UNIT_CTRL & SQRTCON_BUSY) {
  }
  return REG_SQRT_RESULT;
}

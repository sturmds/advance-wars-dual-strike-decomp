#include "types.h"
#include "sdk/svc.h"

/* BIOS call 0x0b (CpuSet, by the caller's arguments). */

void func_arm7_038067b0(const void* src, void* dst, u32 control) {
  Bios_FillOrCopy_Arm7(src, dst, control);
}

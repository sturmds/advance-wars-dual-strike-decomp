#include "types.h"

/* BIOS call 0x0b (CpuSet, by the caller's arguments). */
extern void Bios_FillOrCopy_Arm7(const void* src, void* dst, u32 control);

void func_arm7_038067b0(const void* src, void* dst, u32 control) {
  Bios_FillOrCopy_Arm7(src, dst, control);
}

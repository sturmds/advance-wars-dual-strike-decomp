#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void* dest, u32 data, u32 size);

void MbCache_Init(DlPlayHeldProgram* cache) {
  MI_CpuFill8(cache, 0, sizeof(DlPlayHeldProgram));
}

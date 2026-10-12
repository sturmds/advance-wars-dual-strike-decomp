#include "types.h"
#include "sdk/gx_load.h"

void GX_LoadTexPltt(const void* src, u32 offset, u32 size) {
  void* dst = (void*)(data_022a98a8 + offset);

  if (data_0216d958 != -1) {
    MI_DmaCopy32Async(data_0216d958, src, dst, size, NULL, NULL);
  } else {
    MIi_CpuCopy32(src, dst, size);
  }
}

#include "types.h"
#include "sdk/gx_load.h"

/* Copies with DMA and waits for it (best guess: MI_DmaCopy32). */
void MI_DmaCopy32(int dmaNo, const void* src, void* dst, u32 size);

static inline void GXi_DmaCopy32(int dmaNo, const void* src, void* dst,
                                 u32 size) {
  if (dmaNo != -1 && size > 48) {
    MI_DmaCopy32(dmaNo, src, dst, size);
  } else {
    MIi_CpuCopy32(src, dst, size);
  }
}

static inline void GXi_DmaCopy32Async(int dmaNo, const void* src, void* dst,
                                      u32 size) {
  if (dmaNo != -1) {
    MI_DmaCopy32Async(dmaNo, src, dst, size, NULL, NULL);
  } else {
    MIi_CpuCopy32(src, dst, size);
  }
}

/* Copies texture data to offset in texture image memory. When the mapped
 * banks are not contiguous in LCDC space, a copy that crosses from the
 * first block into the second is split in two. */
void GX_LoadTex(const void* src, u32 offset, u32 size) {
  void* dst;

  if (data_022a98b4 == 0) {
    dst = (void*)(data_022a98a4 + offset);
  } else if (offset + size < data_022a98b8) {
    dst = (void*)(data_022a98a4 + offset);
  } else if (offset >= data_022a98b8) {
    dst = (void*)(data_022a98b4 + offset - data_022a98b8);
  } else {
    void* dst2 = (void*)data_022a98b4;
    u32 size1 = data_022a98b8 - offset;

    dst = (void*)(data_022a98a4 + offset);
    GXi_DmaCopy32(data_0216d958, src, dst, size1);
    GXi_DmaCopy32Async(data_0216d958, (const u8*)src + size1, dst2,
                       size - size1);
    return;
  }
  GXi_DmaCopy32Async(data_0216d958, src, dst, size);
}

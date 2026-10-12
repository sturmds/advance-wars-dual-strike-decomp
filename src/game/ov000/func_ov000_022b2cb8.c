#include "types.h"

void Layers_ClearLeftBlock(const void* src, void* dst, u32 size);

void func_ov000_022b2cb8(const void* src, void* dst, u32 size) {
  Layers_ClearLeftBlock(src, dst, size);
}

#include "types.h"

void SoundOutput_Restart(const void* src, void* dst, u32 size);

void func_02104ddc(const void* src, void* dst, u32 size) {
  SoundOutput_Restart(src, dst, size);
}

#include "sdk/wl.h"

/* Reads one byte through a 16-bit load: the wireless RAM has no byte access. */
u8 WL_ReadByte(const void* addr) {
  u32 value;

  if ((u32)addr & 1) {
    value = (*(const u16*)((u32)addr - 1) >> 8) & 0xff;
  } else {
    value = *(const u16*)addr & 0xff;
  }
  return value;
}

#include "types.h"
#include "sdk/msl_string.h"

#define HAS_NO_ZERO_BYTE(w) ((((w) + 0xFEFEFEFF) & 0x80808080) == 0)

int strcmp(const char* a, const char* b) {
  u32 va;
  u32 vb;
  u32 misalign;
  int delta;

  va = *(u8*)a;
  vb = *(u8*)b;
  delta = va - vb;
  if (delta != 0) {
    return delta;
  }

  misalign = (u32)a & 3;
  if (((u32)b & 3) != misalign) {
    goto bytes;
  }

  if (misalign != 0) {
    if (va == 0) {
      return 0;
    }
    for (misalign = 3 - misalign; misalign != 0; misalign--) {
      va = *(u8*)++a;
      vb = *(u8*)++b;
      delta = va - vb;
      if (delta != 0) {
        return delta;
      }
      if (va == 0) {
        return 0;
      }
    }
    a++;
    b++;
  }

  va = *(u32*)a;
  vb = *(u32*)b;
  if (HAS_NO_ZERO_BYTE(va)) {
    while (va == vb) {
      va = *(u32*)(a += 4);
      vb = *(u32*)(b += 4);
      if (!HAS_NO_ZERO_BYTE(va)) {
        goto last_word;
      }
    }
    a--;
    b--;
    goto bytes;
  }

last_word:
  va = *(u8*)a;
  vb = *(u8*)b;
  delta = va - vb;
  if (delta != 0) {
    return delta;
  }

bytes:
  if (va == 0) {
    return 0;
  }
  do {
    va = *(u8*)++a;
    vb = *(u8*)++b;
    delta = va - vb;
    if (delta != 0) {
      return delta;
    }
  } while (va != 0);
  return 0;
}

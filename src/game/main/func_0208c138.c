#include "types.h"

typedef struct Unk_0208c138_0 Unk_0208c138_0;

struct Unk_0208c138_0 {
  u8 pad_00[0x130];
  int unk_130;
};

extern int data_02297790;
extern Unk_0208c138_0* data_027c0280;

void* func_0208c138(void) {
  data_02297790 = data_027c0280->unk_130;
  return &data_02297790;
}

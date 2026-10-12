#include "types.h"

typedef struct Unk_02290118 Unk_02290118;

struct Unk_02290118 {
  u8 pad_00[0x8];
  u16 unk_8;
};

extern Unk_02290118 data_02290118;

void* func_02103938(void) {
  data_02290118.unk_8 = 1;
  return &data_02290118;
}

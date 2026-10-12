#include "types.h"

typedef struct Unk_02108f2c_0 Unk_02108f2c_0;

struct Unk_02108f2c_0 {
  u8 pad_00[0x148];
  int unk_148;
};

void SoundStreamHandle_Detach(Unk_02108f2c_0** a0) {
  Unk_02108f2c_0* t0 = *a0;
  if ((int)t0 != 0) {
    t0->unk_148 = 0;
    *a0 = 0;
  }
}

#include "types.h"

typedef struct Unk_02357674_0 Unk_02357674_0;

struct Unk_02357674_0 {
  u8 pad_00[0xc];
  u16 unk_c;
};

extern Unk_02357674_0* data_ov006_0236d640;

int func_ov006_02357674(void) {
  Unk_02357674_0* t0 = data_ov006_0236d640;
  u16 t1 = t0->unk_c;
  if (t1 == 51) {
    t0->unk_c = 30;
    return 30;
  }
  return t1;
}

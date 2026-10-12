#include "types.h"

typedef struct Unk_023505c4_0 Unk_023505c4_0;

struct Unk_023505c4_0 {
  u8 pad_00[0xc];
  u16 unk_c;
};

extern Unk_023505c4_0* data_ov006_0236d640;

void func_ov006_023505c4(void) { data_ov006_0236d640->unk_c = 1; }

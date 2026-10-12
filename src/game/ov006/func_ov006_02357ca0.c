#include "types.h"

typedef struct Unk_02357ca0_0 Unk_02357ca0_0;

struct Unk_02357ca0_0 {
  u8 pad_00[0xd8];
  u8 unk_d8;
};

extern Unk_02357ca0_0* data_ov006_0236d640;

void func_ov006_02357ca0(void) { data_ov006_0236d640->unk_d8 = 0; }

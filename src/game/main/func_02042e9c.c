#include "types.h"

typedef struct Unk_02042e9c_0 Unk_02042e9c_0;

struct Unk_02042e9c_0 {
  u8 pad_00[0x31];
  u8 unk_31;
  u8 unk_32;
};

extern Unk_02042e9c_0* data_0229c73c;
void PlayerInfo_StartTaskFor(int, int, int);

void func_02042e9c(int a0) {
  Unk_02042e9c_0* t0 = data_0229c73c;
  PlayerInfo_StartTaskFor(a0, t0->unk_31, t0->unk_32);
}

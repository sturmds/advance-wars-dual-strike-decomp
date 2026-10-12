#include "types.h"

typedef struct Unk_02057ef8_0 Unk_02057ef8_0;
typedef struct Unk_02057ef8_1 Unk_02057ef8_1;
struct Unk_02057ef8_0 {
  u8 pad_00[0x48];
  Unk_02057ef8_1* unk_48[1];
};
struct Unk_02057ef8_1 {
  u8 pad_00[0x2];
  u16 unk_2;
};
extern Unk_02057ef8_0* data_027c027c;
extern int data_027c0284;
void Player_SetUnk33(int, void*);

void func_020dd370(void) {
  Unk_02057ef8_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  Player_SetUnk33(t0->unk_48[t1]->unk_2, t0);
}

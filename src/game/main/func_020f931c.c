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
void Gfx_LoadPlayerBmap0a0Palette(int player);

void func_020f931c(void) {
  Unk_02057ef8_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  Gfx_LoadPlayerBmap0a0Palette(t0->unk_48[t1]->unk_2);
}

#include "types.h"
#include "game/Unk_02080440.h"

void PackNibblePairs(const u8* nibbles, u8* out);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_0229d8c0;
extern u8 data_0229d8c8[];
extern u8 data_0229d8d4[];

void func_020802e8(void) {
  if (data_0229d8c0 == 1) {
    return;
  }
  data_0229d8c8[2] = data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_0229d8c8[3] = data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  data_0229d8c8[4] = data_027c027c->unk_48[data_027c0284]->actingUnitStartX;
  data_0229d8c8[5] = data_027c027c->unk_48[data_027c0284]->actingUnitStartY;
  PackNibblePairs(data_027c027c->unk_48[data_027c0284]->pathSteps,
                  data_0229d8d4);
  data_0229d8c0 = 1;
}

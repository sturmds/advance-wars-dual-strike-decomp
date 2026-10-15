#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

void PackNibblePairs(const u8* nibbles, u8* out);

extern u8 data_0229d8c0;
extern UnitAction data_0229d8c8;
extern u8 data_0229d8d4[];

void func_020802e8(void) {
  if (data_0229d8c0 == 1) {
    return;
  }
  data_0229d8c8.x = data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_0229d8c8.y = data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  data_0229d8c8.fromX = data_027c027c->unk_48[data_027c0284]->actingUnitStartX;
  data_0229d8c8.fromY = data_027c027c->unk_48[data_027c0284]->actingUnitStartY;
  PackNibblePairs(data_027c027c->unk_48[data_027c0284]->pathSteps,
                  data_0229d8d4);
  data_0229d8c0 = 1;
}

#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern u8 data_027c01c0[];

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

void Ov004_QueueBottomSprites(Unk_ov004_022bf158* state) {
  if (state->unk_1c0c == 0) {
    return;
  }
  Sprite_Queue(0, 16, 174, data_027c01c0, 0x1000, 64);
  Sprite_Queue(0, 144, 174, data_027c01c0, 0x1000, 96);
}

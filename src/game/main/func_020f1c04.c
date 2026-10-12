#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

u32 Rng_Next(void);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

void func_020f1c04(Unk_021694c0Task* task) {
  Unk_021694c0* scene = data_021694c0;
  int x = scene->unk_24 + scene->unk_2c + ((Rng_Next() >> 2) & 63) - 32;
  Unk_021694c0* scene2 = data_021694c0;
  int y = scene2->unk_28 + scene2->unk_30 + ((Rng_Next() >> 2) & 63) - 32;

  if (task->frame == 20) {
    task->frame = 0;
    if (y > 0 && y < 192) {
      SpriteAnimTask_Start(data_0213b8f0, x, y, 0x4170, 5, 0);
    }
  }
  task->frame++;
}

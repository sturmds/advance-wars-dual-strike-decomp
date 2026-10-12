#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

int MainLoop_GetFrameCount(void);
u32 Rng_Next(void);
int func_020f2280(void);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);
void SpriteAnim_Draw(struct ShowSprite* sprite, int x, int y);

/* Per-frame step of the effect scene task: while func_020f2280 holds, adds a
 * spark sprite every 8th frame at a random spot within 16 pixels of the scene
 * position, then keeps the task's own sprite on that position. */
void EffectScene_SparkStep(Unk_021694c0Task* task) {
  if (func_020f2280() != 0 && MainLoop_GetFrameCount() % 8 == 0) {
    Unk_021694c0* scene = data_021694c0;
    SpriteAnimTask_Start(
        data_0213b8f0,
        scene->unk_24 + scene->unk_2c + ((Rng_Next() >> 2) & 31) - 16,
        scene->unk_28 + scene->unk_30 + ((Rng_Next() >> 2) & 31) - 16, 0x4170,
        6, 0);
  }
  SpriteAnim_Draw(task->sprite,
                  (data_021694c0->unk_24 + data_021694c0->unk_2c) & 0x1ff,
                  (data_021694c0->unk_28 + data_021694c0->unk_30) & 0xff);
}

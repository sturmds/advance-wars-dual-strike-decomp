#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

BOOL SpriteFile_Queue(void* set, int index, int arg, int x, int y, int attr,
                      int tile, int list);

/* task is unused; EffectScene_Update passes it. */
void EffectScene_QueueSprite(Unk_021694c0Task* task) {
  SpriteFile_Queue(data_0213b8f0, 0, data_021694c0->effectFrames,
                   (data_021694c0->unk_24 + data_021694c0->unk_2c) & 0x1ff,
                   (data_021694c0->unk_28 + data_021694c0->unk_30) & 0xff,
                   0x4170, 0, 0);
}

#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

void Sound_PlaySubSe(u32 soundId);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

void func_020f2014(void) {
  Sound_PlaySubSe(181);
  SpriteAnimTask_Start(
      data_0213b8f0, data_021694c0->unk_24 + data_021694c0->unk_2c,
      data_021694c0->unk_28 + data_021694c0->unk_30, 0x4170, 7, 0);
  SpriteAnimTask_Start(
      data_0213b8f0, data_021694c0->unk_24 + data_021694c0->unk_2c,
      data_021694c0->unk_28 + data_021694c0->unk_30, 0x4170, 3, 0);
}

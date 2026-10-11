#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;
extern u8 data_0213b8f0[];

void Sound_PlaySubSe(u32 soundId);
void *SpriteAnimTask_Start(void *file, int x, int y, int id, int a, int b);

void func_020f2014(void)
{
    Sound_PlaySubSe(181);
    SpriteAnimTask_Start(data_0213b8f0, data_021694c0->unk_24 + data_021694c0->unk_2c,
                  data_021694c0->unk_28 + data_021694c0->unk_30, 0x4170, 7, 0);
    SpriteAnimTask_Start(data_0213b8f0, data_021694c0->unk_24 + data_021694c0->unk_2c,
                  data_021694c0->unk_28 + data_021694c0->unk_30, 0x4170, 3, 0);
}

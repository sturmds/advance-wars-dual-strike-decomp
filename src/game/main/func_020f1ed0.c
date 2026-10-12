#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/unit_show_task.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);

void func_020f1ed0(Unk_021694c0Task* task) {
  task->sprite = SpriteAnim_Create(data_0213b8f0, 0, 2);
  SpriteAnim_SetAnimation(task->sprite, 4);
  task->sprite->unk_24 = 0x4170;
  task->frame = 0;
  task->x = data_021694c0->unk_24;
  task->y = 192;
}

#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/unit_show_task.h"

extern u8 data_0213b8ac[];

ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);

void EffectScene_InitSprite(Unk_021694c0Task* task) {
  task->easeFrom = 0;
  task->sprite = SpriteAnim_Create(data_0213b8ac, 0, 2);
  SpriteAnim_SetAnimation(task->sprite, 0);
  task->sprite->unk_24 = 0x2070;
}

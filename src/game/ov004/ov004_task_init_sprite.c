#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/unit_show_task.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);

void Ov004Task_InitSprite(Ov004Task* task) {
  ShowSprite* sprite;

  /* A slot of -1 sends the task to step 100 without setting a frame. */
  task->sprite = SpriteAnim_Create(
      (u8*)data_ov004_022bf158->buffer + OV004_SPRITE_GRAPHICS_OFFSET, 0, 2);
  sprite = task->sprite;
  if (task->slot == -1) {
    task->step = 100;
    return;
  }
  SpriteAnim_SetAnimation(sprite, task->unk_4c);
  sprite->unk_24 = 0x400;
  task->step = 0;
}

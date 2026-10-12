#include "types.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/silo_missile_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const u8 data_0213dd38[];

SpriteAnim* SpriteAnim_Create(const void* file, int a, int b);
void SpriteAnim_SetAnimation(void* sprite, int animation);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step that starts the falling missile: the target in map pixels,
 * the sprite, and the first row, which is the top of the map view. */
void SiloMissileFall_Init(SiloMissileTask* task) {
  task->pixelX = task->x * 16 + 8;
  task->pixelY = task->y * 16 + 16;
  task->fallY = STATE->unk_1c4->scrollY;
  task->sprite = SpriteAnim_Create(data_0213dd38, 1, 2);
  SpriteAnim_SetAnimation(task->sprite, 2);
  task->sprite->tileBase = task->spriteAttr;
}

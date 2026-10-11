#include "types.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/silo_missile_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const u8 data_0213dd38[];

SpriteAnim *SpriteAnim_Create(const void *file, int a, int b);
void SpriteAnim_SetAnimation(void *sprite, int animation);
void Sound_PlaySe(u32 soundId);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step that starts the launching missile: turns the tile into map
 * pixels, starts a one-shot sprite animation there (a SpriteAnimTask),
 * creates the missile sprite and plays sound 173. */
void SiloMissileLaunch_Init(SiloMissileTask *task)
{
    task->x = task->x * 16 + 8;
    task->y = task->y * 16 + 16;
    SpriteAnimTask_Start(data_0213dd38, task->x - STATE->unk_1c4->scrollX,
                         task->y - STATE->unk_1c4->scrollY, task->spriteAttr, 0, 1);
    task->sprite = SpriteAnim_Create(data_0213dd38, 1, 2);
    SpriteAnim_SetAnimation(task->sprite, 1);
    task->sprite->tileBase = task->spriteAttr;
    Sound_PlaySe(173);
}

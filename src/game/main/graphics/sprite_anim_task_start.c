#include "types.h"
#include "game/task.h"
#include "game/sprite_anim.h"

extern const TaskScriptCmd data_02158838[];

SpriteAnim *func_02022478(const void *file, u16 layer, int flags);
void SpriteAnim_SetAnimation(SpriteAnim *sprite, int animation);

/* Loads a sprite from a halfword-offset file, selects animation, and starts
 * a task (data_02158838) that draws it at (x, y) each frame. */
void SpriteAnimTask_Start(const void *file, int x, int y, u32 tileBase, int animation, int layer)
{
    SpriteAnim *sprite;
    SpriteAnimTask *task;

    sprite = func_02022478(file, layer, SPRITE_ANIM_HALFWORD_FILE);
    SpriteAnim_SetAnimation(sprite, animation);
    sprite->tileBase = tileBase;
    task = (SpriteAnimTask *)Task_Create(data_02158838, TASK_LIST(3));
    task->sprite = sprite;
    task->x = x;
    task->y = y;
}

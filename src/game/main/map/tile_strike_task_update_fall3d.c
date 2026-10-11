#include "types.h"
#include "game/tile_strike_task.h"

void Task_ResumeScript(TileStrikeTask *task);
void Sound_PlaySe(int songId);
void ScreenShake_Start(int tableIndex, int steps, void *parent);
void SpriteAnim_SetAnimation(struct SpriteAnim *sprite, int animation);
int SpriteAnim_Draw3d(struct SpriteAnim *sprite, TileStrikePos *pos, TileStrikePos *rot);

/* The sprite falls 10 units a frame from height 100. At 0 it switches to
 * the impact animation, with a sound and a screen shake. */
void TileStrikeTask_UpdateFall3d(TileStrikeTask *task)
{
    TileStrikePos pos;

    pos.x = (task->x << 13) / 256;
    pos.y = (task->y - task->height) * 0x6000 / 0x300;
    pos.z = (task->height << 13) / 256;
    SpriteAnim_Draw3d(task->sprite, &pos, NULL);
    task->height -= 10;
    if (task->height > 0) {
        return;
    }
    SpriteAnim_SetAnimation(task->sprite, 1);
    task->y = task->tileY * 16 + 8;
    Sound_PlaySe(159);
    ScreenShake_Start(0, 8, NULL);
    Task_ResumeScript(task);
}

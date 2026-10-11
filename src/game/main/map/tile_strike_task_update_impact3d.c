#include "types.h"
#include "game/tile_strike_task.h"

void Task_ResumeScript(TileStrikeTask *task);
int SpriteAnim_Draw3d(struct SpriteAnim *sprite, TileStrikePos *pos, TileStrikePos *rot);

/* Draws the sprite in 3D at its tile; the script resumes when the draw
 * call returns 0 (taken to mean the animation ended). */
void TileStrikeTask_UpdateImpact3d(TileStrikeTask *task)
{
    TileStrikePos pos;

    pos.x = (task->x << 13) / 256;
    pos.y = task->y * 0x6000 / 0x300;
    pos.z = task->height;
    if (!SpriteAnim_Draw3d(task->sprite, &pos, NULL)) {
        Task_ResumeScript(task);
    }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_strike_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void Task_ResumeScript(TileStrikeTask *task);
int SpriteAnim_Draw(struct SpriteAnim *sprite, int x, int y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Draws the landed sprite against the map scroll; the script resumes when
 * its draw call returns 0 (taken to mean the animation ended). */
void TileStrikeTask_UpdateImpact2d(TileStrikeTask *task)
{
    if (!SpriteAnim_Draw(task->sprite, (task->x - GAME->unk_1c4->scrollX) & 0x1ff, (task->y - GAME->unk_1c4->scrollY) & 0xff)) {
        Task_ResumeScript(task);
    }
}

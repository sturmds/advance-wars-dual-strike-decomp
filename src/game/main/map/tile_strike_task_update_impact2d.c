#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_strike_task.h"

void Task_ResumeScript(TileStrikeTask* task);
int SpriteAnim_Draw(struct SpriteAnim* sprite, int x, int y);

/* Draws the landed sprite against the map scroll; the script resumes when
 * its draw call returns 0 (taken to mean the animation ended). */
void TileStrikeTask_UpdateImpact2d(TileStrikeTask* task) {
  if (!SpriteAnim_Draw(task->sprite,
                       (task->x - MAP_STATE->unk_1c4->scrollX) & 0x1ff,
                       (task->y - MAP_STATE->unk_1c4->scrollY) & 0xff)) {
    Task_ResumeScript(task);
  }
}

#include "types.h"
#include "game/co_rank_task.h"
#include "game/win_quote.h"

/* Script step: draws the portrait of the front CO at x 64 and, in a tag
 * battle, the back CO at x 192, mirrored. */
void CoRankTask_DrawPortraits(CoRankTask* task) {
  Sprite_QueueCoPortrait(64, 160, 0x6800, 0x400, 10, FALSE);
  if (task->hasBackCo == 1) {
    Sprite_QueueCoPortrait(192, 160, 0x7800, 0x5b0, 11, TRUE);
  }
}

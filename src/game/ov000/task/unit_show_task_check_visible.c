#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern void* data_027c0288;
extern int gLinkPlay;

BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, int x, int y);
void MapView_ScrollToPixel(int x, int y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* TRUE when the task's unit is worth showing: on the other screen it has to
 * be inside the view, and on the current one the tile has to be in sight (or
 * the current player has Player.visionGrids bit 1 set). A task in the move state
 * first scrolls the view to its unit, except in link play for unk_18 == 21.
 * The goto is needed to match: it keeps both returns as separate blocks. */
BOOL UnitShowTask_CheckVisible(UnitShowTask* task) {
  if ((void*)data_027c027c != data_027c0288) {
    if (task->y - GAME->unk_1c4->scrollY < -8 ||
        task->y - GAME->unk_1c4->scrollY > 192 ||
        task->x - GAME->unk_1c4->scrollX < -8 ||
        task->x - GAME->unk_1c4->scrollX > 256) {
      return FALSE;
    }
  }
  if (task->state == UNIT_SHOW_STATE_MOVE) {
    if ((gLinkPlay == 0) | (gLinkPlay != 0 ? GAME->unk_18 != 21 : 0)) {
      MapView_ScrollToPixel(task->x, task->y);
    }
  }
  if ((GAME->players[GAME->currentPlayer].visionGrids & 2) ||
      GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[(task->y + 8) / 16] +
                               (task->x + 8) / 16] != 0) {
    if (Unit_IsVisibleToActivePlayers(task->unit, (s16)((task->x + 8) / 16),
                                      (s16)((task->y + 8) / 16))) {
      goto shown;
    }
  }
  return FALSE;
shown:
  return TRUE;
}

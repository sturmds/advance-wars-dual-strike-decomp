#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

extern u32 gLinkPlay;

BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, int x, int y);
void MapView_ScrollToPixel(int x, int y);

/* TRUE when the task's unit is worth showing: on the other screen it has to
 * be inside the view, and on the current one the tile has to be in sight (or
 * the current player has Player.visionGrids bit 1 set). A task in the move state
 * first scrolls the view to its unit, except in link play for unk_18 == 21.
 * The goto is needed to match: it keeps both returns as separate blocks. */
BOOL UnitShowTask_CheckVisible(UnitShowTask* task) {
  if ((void*)data_027c027c != data_027c0288.unk_00) {
    if (task->y - MAP_STATE->unk_1c4->scrollY < -8 ||
        task->y - MAP_STATE->unk_1c4->scrollY > 192 ||
        task->x - MAP_STATE->unk_1c4->scrollX < -8 ||
        task->x - MAP_STATE->unk_1c4->scrollX > 256) {
      return FALSE;
    }
  }
  if (task->state == UNIT_SHOW_STATE_MOVE) {
    if ((gLinkPlay == 0) | (gLinkPlay != 0 ? MAP_STATE->unk_18 != 21 : 0)) {
      MapView_ScrollToPixel(task->x, task->y);
    }
  }
  if ((MAP_STATE->players[MAP_STATE->currentPlayer].visionGrids & 2) ||
      MAP_STATE->mapTiles
              ->unk_2338[MAP_STATE->mapTiles->rowStart[(task->y + 8) / 16] +
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

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void Gfx_LoadPlayerWindowFrame(int player);

/* Last callback of the Start-button map overview script (gMapOverviewScript).
 * When the view scrolled during the overview, puts the cursor at the middle
 * of the view (7 tiles right and 5 down of its corner), then reloads the
 * current player's window frame. */
void MapOverview_RestoreCursor(MapViewSlot* slot) {
  if (slot->scrollX != MAP_STATE->unk_1c4->scrollX ||
      slot->scrollY != MAP_STATE->unk_1c4->scrollY) {
    MAP_STATE->cursorX = (MAP_STATE->unk_1c4->scrollX >> 4) + 7;
    MAP_STATE->cursorY = (MAP_STATE->unk_1c4->scrollY >> 4) + 5;
  }
  Gfx_LoadPlayerWindowFrame(MAP_STATE->currentPlayer);
  MAP_STATE->unk_1c = 0;
}

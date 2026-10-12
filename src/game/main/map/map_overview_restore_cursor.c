#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Gfx_LoadPlayerWindowFrame(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Last callback of the Start-button map overview script (gMapOverviewScript).
 * When the view scrolled during the overview, puts the cursor at the middle
 * of the view (7 tiles right and 5 down of its corner), then reloads the
 * current player's window frame. */
void MapOverview_RestoreCursor(MapViewSlot* slot) {
  if (slot->scrollX != STATE->unk_1c4->scrollX ||
      slot->scrollY != STATE->unk_1c4->scrollY) {
    STATE->cursorX = (STATE->unk_1c4->scrollX >> 4) + 7;
    STATE->cursorY = (STATE->unk_1c4->scrollY >> 4) + 5;
  }
  Gfx_LoadPlayerWindowFrame(STATE->currentPlayer);
  STATE->unk_1c = 0;
}

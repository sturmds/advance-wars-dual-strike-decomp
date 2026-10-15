#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

/* Hold function set by EventCmd_HoldUntilTurn: in game state 14, clears the
 * slot's hold when the day matches (a negative day matches any) and the
 * slot's player is the current player. */
void EventHold_ReleaseOnTurn(EventScriptSlot* slot) {
  if (MAP_STATE->unk_18 != EVENT_GAME_STATE_14) {
    return;
  }
  if (slot->day >= 0 && slot->day != MAP_STATE->day) {
    return;
  }
  if (slot->player == MAP_STATE->currentPlayer) {
    slot->hold = 0;
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsPowerActive(int player);

/* Hold function set by EventCmd_HoldUntilPower: in game state 14, clears
 * the slot's hold when the slot's player has a CO power active and player
 * 1 is the current player. */
void EventHold_ReleaseOnPower(EventScriptSlot* slot) {
  if (STATE->unk_18 != EVENT_GAME_STATE_14) {
    return;
  }
  if (!Player_IsPowerActive(slot->player)) {
    return;
  }
  if (STATE->currentPlayer == 1) {
    slot->hold = 0;
  }
}

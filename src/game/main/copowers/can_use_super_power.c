#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int SuperPowerCost(int player, int slot);
BOOL Player_IsPowerActive(int player);

/* TRUE when the CO in `slot` (0 front, 1 back) has enough charge for its
 * Super CO Power and no power is on yet. */
BOOL CanUseSuperPower(int player, int slot) {
  u32 cost;
  Player* p;

  if (Player_IsPowerActive(player)) {
    return FALSE;
  }
  cost = SuperPowerCost(player, slot);
  p = &data_027c027c->unk_48[data_027c0284]->players[player];
  /* front and back are adjacent words: slot picks one */
  return (&p->front)[slot].charge >= cost;
}

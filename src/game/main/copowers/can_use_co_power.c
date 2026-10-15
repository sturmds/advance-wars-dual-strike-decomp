#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int CoPowerCost(int player, int slot);
BOOL Player_IsPowerActive(int player); /* the front CO has a power on */
BOOL CanUseSuperPower(int player,
                      int slot); /* the same test for the Super CO Power */

/* TRUE when the CO in `slot` (0 front, 1 back) has enough charge for its
 * CO Power and no power is on yet. A CO whose CO Power costs nothing has
 * only a Super CO Power, so the Super test decides. */
BOOL CanUseCoPower(int player, int slot) {
  u32 cost;
  Player* p;

  if (Player_IsPowerActive(player)) {
    return FALSE;
  }
  if (CoPowerCost(player, slot) == 0) {
    return CanUseSuperPower(player, slot);
  }
  cost = CoPowerCost(player, slot);
  p = &data_027c027c->unk_48[data_027c0284]->players[player];
  /* front and back are adjacent words: slot picks one */
  return (&p->front)[slot].charge >= cost;
}

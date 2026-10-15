#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int CoPowerCost(int player, int slot);
BOOL Player_IsPowerActive(int player);

/* Like CanUseCoPower, but a CO whose CO Power costs nothing (Super CO Power
 * only) gets FALSE instead of the Super test. */
BOOL CanUseCoPowerStrict(int player, int slot) {
  u32 cost;
  Player* p;

  if (Player_IsPowerActive(player)) {
    return FALSE;
  }
  if (CoPowerCost(player, slot) == 0) {
    return FALSE;
  }
  cost = CoPowerCost(player, slot);
  p = &data_027c027c->unk_48[data_027c0284]->players[player];
  /* front and back are adjacent words: slot picks one */
  return (&p->front)[slot].charge >= cost;
}

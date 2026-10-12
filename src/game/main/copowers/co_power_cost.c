#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int PowerStarCharge(int player, int slot);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Meter charge a CO Power costs for the CO in a player's slot (0 front, 1
 * back): the CO's CO Power star count times the charge per star. */
int CoPowerCost(int player, int slot) {
  int starCharge = PowerStarCharge(player, slot);
  u8 co =
      (&data_027c027c->unk_48[data_027c0284]->players[player].front)[slot].id;

  return starCharge * gCoTable[co & PLAYER_CO_ID_MASK].powerStars;
}

#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

int RepairIncrease(int player, int slot);

/* Extra repair of one CO of a player (slot 0 front, 1 back) in a power state:
 * the CO's repairBonus (0 with coEffectsOn clear) plus the Force Rank bonus. */
int Player_GetCoRepairBonus(int playerIndex, int slot, int powerState) {
  int bonus = 0;
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  int co = (&state->players[playerIndex].front)[slot].id & PLAYER_CO_ID_MASK;

  if (state->coEffectsOn != 0) {
    bonus += gCoTable[co].blocks[powerState].repairBonus;
  }
  return bonus + RepairIncrease(playerIndex, slot);
}

#include "types.h"

int Player_GetCoRepairBonus(int playerIndex, int slot, int powerState);

/* Repair per turn in shown hp: 2 plus the CO's bonus. */
int Player_GetRepairHp(int playerIndex, int slot, int powerState) {
  return Player_GetCoRepairBonus(playerIndex, slot, powerState) + 2;
}

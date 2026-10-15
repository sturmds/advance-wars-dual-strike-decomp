#include "types.h"
#include "game/Unk_02080440.h"

int Player_GetRepairHp(int playerIndex, int slot, int powerState);

/* Shown HP a property repairs for the player, with the front CO's current
 * power state. */
int Player_GetDailyRepairHp(int player) {
  return Player_GetRepairHp(player, 0,
                            MAP_STATE->players[player].front.powerState);
}

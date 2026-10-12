#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Player_GetRepairHp(int playerIndex, int slot, int powerState);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Shown HP a property repairs for the player, with the front CO's current
 * power state. */
int Player_GetDailyRepairHp(int player) {
  return Player_GetRepairHp(player, 0, STATE->players[player].front.powerState);
}

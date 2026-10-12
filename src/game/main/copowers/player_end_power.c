#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Switches the player's power off: the front CO goes back to day-to-day
 * and the firepower and defense bonuses of the power are cleared. */
void Player_EndPower(int player) {
  data_027c027c->unk_48[data_027c0284]->players[player].front.powerState =
      POWER_NONE;
  data_027c027c->unk_48[data_027c0284]->players[player].firepowerBonus = 0;
  data_027c027c->unk_48[data_027c0284]->players[player].defenseBonus = 0;
}

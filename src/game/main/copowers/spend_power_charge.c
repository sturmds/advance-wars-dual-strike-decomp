#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int SuperPowerCost(int player, int slot);
int CoPowerCost(int player, int slot);

/* Takes the cost of a power (enum CoPowerState) from the player's front
 * CO meter, stopping at 0. CoPowerCost gives the CO Power cost. */
void SpendPowerCharge(int player, int power) {
  int cost;
  Player* p;
  int charge;

  switch (power) {
    case POWER_CO:
    default:
      cost = CoPowerCost(player, 0);
      break;
    case POWER_SUPER:
      cost = SuperPowerCost(player, 0);
      break;
  }
  p = &data_027c027c->unk_48[data_027c0284]->players[player];
  charge = p->front.charge - cost;
  if (charge < 0) {
    charge = 0;
  }
  p->front.charge = charge;
}

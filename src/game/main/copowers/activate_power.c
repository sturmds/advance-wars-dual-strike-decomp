#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void CountPowerUse(int player);
void SpendPowerCharge(int player, int power);
void Power_StartTasks(int player, int power, BOOL startSecond);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Starts a power (enum CoPowerState) for the player's front CO: counts
 * the use, takes the meter charge, clears the ready bits and starts the
 * power's tasks. */
void ActivatePower(int player, int power) {
  CountPowerUse(player);
  SpendPowerCharge(player, power);
  data_027c027c->unk_48[data_027c0284]->players[player].front.powerReady = 0;
  if (data_027c027c->unk_48[data_027c0284]->players[player].tagPowerHalf !=
      TAG_POWER_BACK) {
    Power_StartTasks(player, power, TRUE);
  } else {
    Power_StartTasks(player, power, FALSE);
  }
}

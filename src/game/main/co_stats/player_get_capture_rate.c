#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Co_GetCapturePercent(int co, int powerState);

/* Capture rate in percent of a player's front CO in its current power state. */
int Player_GetCaptureRate(int playerIndex) {
  return Co_GetCapturePercent(
      data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
          PLAYER_CO_ID_MASK,
      data_027c027c->unk_48[data_027c0284]
          ->players[playerIndex]
          .front.powerState);
}

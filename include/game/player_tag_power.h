#ifndef GAME_PLAYER_TAG_POWER_H
#define GAME_PLAYER_TAG_POWER_H

#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

#define PLAYER_TAG(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* Either half of a tag power is running. */
static inline BOOL Player_InTagPower(int playerIndex) {
  return PLAYER_TAG(playerIndex).tagPowerHalf == TAG_POWER_FRONT ||
         PLAYER_TAG(playerIndex).tagPowerHalf == TAG_POWER_BACK;
}

/* The player has a back CO and a tag power is running. */
static inline BOOL Player_TagPowerOn(int playerIndex) {
  BOOL on = FALSE;

  if (PLAYER_TAG(playerIndex).back.id != 0 && Player_InTagPower(playerIndex)) {
    on = TRUE;
  }
  return on;
}

#endif

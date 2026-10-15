#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

/* Whole seconds on the timers (Player.playTimeFrames, frames) of player 1 and every
 * player on its team. */
u32 Team_GetSecondsUsed(int unused) {
  u32 seconds;
  int slot;

  seconds = 0;
  for (slot = 1; slot < ARMY_SLOT_COUNT; slot++) {
    if (MAP_STATE->players[1].team == MAP_STATE->players[slot].team) {
      seconds += HwDiv32(MAP_STATE->players[slot].playTimeFrames, 60);
    }
  }
  return seconds;
}

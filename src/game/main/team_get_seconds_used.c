#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

s32 HwDiv32(s32 num, s32 den);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Whole seconds on the timers (Player.playTimeFrames, frames) of player 1 and every
 * player on its team. */
u32 Team_GetSecondsUsed(int unused) {
  u32 seconds;
  int slot;

  seconds = 0;
  for (slot = 1; slot < 5; slot++) {
    if (STATE->players[1].team == STATE->players[slot].team) {
      seconds += HwDiv32(STATE->players[slot].playTimeFrames, 60);
    }
  }
  return seconds;
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/script_slot.h"

extern ScriptCommand gMapOverviewScript[];

s16 ScriptSlot_Find(const ScriptCommand* script);
int func_020d1bc4(void);
int MapScript_RunList18(int flag);
void Counter_IncrementCapped(u32* timer);
void EffectScene_TickProgress(void);

#define CURRENT_PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Per-frame tick of the current player's timer (Player.playTimeFrames, frames; Time
 * Survival reads it). Skipped while the hook script runs, func_020d1bc4 is
 * nonzero, unk_5b is set or map script list 18 takes the frame. Then
 * EffectScene_TickProgress runs. */
void Game_TickPlayerTimer(void) {
  if (ScriptSlot_Find(gMapOverviewScript) != -1) {
    return;
  }
  if (func_020d1bc4() != 0) {
    return;
  }
  if (MAP_STATE->unk_5b != 0) {
    return;
  }
  if (MapScript_RunList18(0) != 0) {
    return;
  }
  Counter_IncrementCapped(&CURRENT_PLAYER.playTimeFrames);
  EffectScene_TickProgress();
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern EventScriptCommand gMapOverviewScript[];

s16 ScriptSlot_Find(const EventScriptCommand *script);
int func_020d1bc4(void);
int MapScript_RunList18(int flag);
void Counter_IncrementCapped(u32 *timer);
void EffectScene_TickProgress(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_PLAYER (STATE->players[STATE->currentPlayer])

/* Per-frame tick of the current player's timer (Player.playTimeFrames, frames; Time
 * Survival reads it). Skipped while the hook script runs, func_020d1bc4 is
 * nonzero, unk_5b is set or map script list 18 takes the frame. Then
 * EffectScene_TickProgress runs. */
void Game_TickPlayerTimer(void)
{
    if (ScriptSlot_Find(gMapOverviewScript) != -1) {
        return;
    }
    if (func_020d1bc4() != 0) {
        return;
    }
    if (STATE->unk_5b != 0) {
        return;
    }
    if (MapScript_RunList18(0) != 0) {
        return;
    }
    Counter_IncrementCapped(&CURRENT_PLAYER.playTimeFrames);
    EffectScene_TickProgress();
}

#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_turn_state.h"
#include "game/ai/ai_show_action.h"
#include "game/ai/ai_turn_run.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_end_actions.h"
#include "game/ai/ai_power.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

extern u32 gLinkPlay;

void func_020ddb38(void);
void Link_SendWeather(int arg);
BOOL EventScript_IsRunning(void);

/* Clears the AI fields of a unit: no target, no service or transport wanted,
 * no landmass claim, and three bits of unknown meaning (unk_08_7, unk_0c_5,
 * unk_0c_6). */
void MapUnit_ResetAiState(MapUnit* unit) {
  unit->aiTargetX = MAP_UNIT_NO_TARGET;
  unit->aiTargetY = MAP_UNIT_NO_TARGET;
  unit->serviceWanted = 0;
  unit->transportWanted = 0;
  unit->unk_08_7 = 0;
  unit->unk_0c_5 = 0;
  unit->unk_0c_6 = 0;
  unit->claimedLandmass = 0;
}

/* Resets the AI fields of slots 1 to 50 of the given player's units. */
void Player_ResetUnitsAiState(int player) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    MapUnit* unit = &data_027c027c->unk_48[data_027c0284]->units[slot];

    if (unit->type != UNIT_NONE) {
      MapUnit_ResetAiState(unit);
    }
  }
}

/* Runs one step of the AI turn. Does nothing while EventScript_IsRunning
 * returns nonzero. */
void AiTurn_Update(void) {
  if (EventScript_IsRunning()) {
    return;
  }
  switch (gAiTurnState->state) {
    case AI_STATE_START:
      AiTurn_Start();
      break;
    case AI_STATE_RUN_PHASE:
      AiTurn_RunPhase();
      break;
    case AI_STATE_RUN_QUEUE:
      AiTurn_RunQueue();
      break;
    case AI_STATE_SHOW_ACTION:
      AiTurn_ShowAction();
      break;
    case AI_STATE_END_ACTIONS:
      AiTurn_RunEndActions();
      break;
    case 5:
      AiTurn_RunPowerStep();
      break;
    case 6:
      AiTurn_Restart();
      break;
  }
}

/* AI state 6: runs func_020ddb38 and goes back to AI_STATE_START. The name
 * is a guess from that; Link_SendWeather runs only when gLinkPlay is set. */
void AiTurn_Restart(void) {
  func_020ddb38();
  gAiTurnState->state = AI_STATE_START;
  if (gLinkPlay != 0) {
    Link_SendWeather(
        data_027c027c->unk_48[data_027c0284]->unk_1c4->nextWeather);
  }
}

int AiTurn_GetState(void) { return gAiTurnState->state; }

/* Sets gAiTurnState->state (AI_STATE_*). */
void AiTurn_SetState(int state) { gAiTurnState->state = state; }

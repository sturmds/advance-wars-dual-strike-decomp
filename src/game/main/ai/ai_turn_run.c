#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_turn_setup.h"
#include "game/ai/ai_turn_marks.h"
#include "game/ai/ai_turn_run.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* The AI unit action steps, one per phase: without fog, and with fog. */
extern void (*gAiUnitPhases[])(void);
extern void (*gAiUnitPhasesFog[])(void);

int Rng_Next(void);
void Rng_SetState(u32 state);
int MainLoop_GetFrameCount(void);
int func_0209dec0(void);
int Player_TeamHasNoBlackHole(void);

/* Runs the step for the current phase, then moves to the next phase. */
void AiTurn_RunPhase(void) {
  Unk_02080440_1c4* map = data_027c027c->unk_48[data_027c0284]->unk_1c4;

  /* Rain brings fog too. */
  if (map->fog == 0 && map->weather != WEATHER_RAIN) {
    gAiUnitPhases[gAiTurnState->phase]();
  } else {
    gAiUnitPhasesFog[gAiTurnState->phase]();
  }
  gAiTurnState->phase++;
}

/* Sets up an AI turn: picks the player's first unit, reseeds the RNG,
 * runs the setup routines, then starts the unit phases. */
void AiTurn_Start(void) {
  Unk_02080440_48* state;

  data_027c027c->unk_48[data_027c0284]->actingUnitIndex =
      Ov000_gPlayerFirstUnitSlot[data_027c027c->unk_48[data_027c0284]
                                     ->currentPlayer];
  state = data_027c027c->unk_48[data_027c0284];
  state->actingUnit = &state->units[state->actingUnitIndex];

  Rng_SetState(Rng_Next() + MainLoop_GetFrameCount());

  AiProfile_BuildForPlayer(data_027c027c->unk_48[data_027c0284]->currentPlayer);
  AiTurn_FindUnitDomains();
  AiTurn_ClearCaptureMarks();
  AiTurn_MarkReachableTiles();
  AiTurn_ClearUnitMarks();
  Ai_ClearTransportsSentToUnit();
  AiTurn_CountCaptureTargets();
  func_0208d964();
  AiTurn_UpdateUnitModes();

  if (func_0209dec0() == 1) {
    gAiTurnState->flags |= 4;
  } else {
    gAiTurnState->flags &= ~4;
  }
  if (Player_TeamHasNoBlackHole() == 1) {
    gAiTurnState->flags |= 8;
  } else {
    gAiTurnState->flags &= ~8;
  }
  gAiTurnState->phase = 0;
  gAiTurnState->actionStep = 0;
  gAiTurnState->state = AI_STATE_RUN_PHASE;
}

void func_0208d964(void) {
  data_021619c0->step = 0;
  data_021619c0->buildStep = 0;
}

void AiTurn_ClearCaptureMarks(void) {
  Unk_02080440_6088* entry;
  int i;

  entry = data_027c027c->unk_48[data_027c0284]->unk_6088;
  for (i = 0; i < MAP_SITE_COUNT; i++) {
    entry->claimCount = 0;
    entry->unk_04 = 0;
    entry->unk_05 = 0;
    entry++;
  }
}

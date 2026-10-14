#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Rng_Next(void);
void Rng_SetState(u32 state);
int MainLoop_GetFrameCount(void);
void AiProfile_BuildForPlayer(int player);
void AiTurn_FindUnitDomains(void);
void AiTurn_ClearCaptureMarks(void);
void AiTurn_MarkReachableTiles(void);
void AiTurn_ClearUnitMarks(void);
void Ai_ClearTransportsSentToUnit(void);
void AiTurn_CountCaptureTargets(void);
void func_0208d964(void);
void AiTurn_UpdateUnitModes(void);
int func_0209dec0(void);
int Player_TeamHasNoBlackHole(void);

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

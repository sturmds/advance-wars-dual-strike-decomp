#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void UnitShowTask_Create(MapUnit* unit);
void func_020c8888(void);
void UnpackNibblePairs(u8* packed, u8* nibbles);
void func_020c8290(u8* nibbles);

/* Unpacks the nibbles saved in gUnitAction.packed back into
 * Unk_02080440_48.pathSteps, then moves to step 2. */
void AiShowAction_Step1(void) {
  UnitShowTask_Create(GAME->actingUnit);
  func_020c8888();
  UnpackNibblePairs(gUnitAction.packed, GAME->pathSteps);
  func_020c8290(GAME->pathSteps);
  gAiTurnState->actionStep = 2;
}

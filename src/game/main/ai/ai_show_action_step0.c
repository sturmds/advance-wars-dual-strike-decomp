#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_0229d8c4;
extern u32 gLinkPlay;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Rng_SetState(u32 state);
int Link_GetOwnPlayer(void);
int func_020b9664(void);
void func_020f5888(int x, int y);

/* First display step. Restores the RNG state the action was made with,
 * then picks the next step by gUnitAction.action: actions 15 to 18 and 27
 * go straight to step 2, 1, 13, 14 and 19 to 21 do their small update and
 * go to step 2, and every other action makes the saved unit the acting unit
 * with its start and end tiles and goes to step 1. */
void AiShowAction_Step0(void) {
  Rng_SetState(gUnitAction.rngState);
  switch (gUnitAction.action) {
    case 15:
    case 16:
    case 17:
    case 18:
    case 27:
      gAiTurnState->actionStep = 2;
      return;
  }
  if (gUnitAction.action == 1 || gUnitAction.action == 14 ||
      gUnitAction.action == 13 || (u8)(gUnitAction.action + 237) <= 2) {
    if (gUnitAction.action == 19) {
      GAME->actingUnitIndex = gUnitAction.unitIndex;
      GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
      GAME->actingUnit->fuel = gUnitAction.fuelLeft;
    }
    if (gUnitAction.action == 20) {
      GAME->actingUnitIndex = gUnitAction.unitIndex;
      GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
      func_020f5888(GAME->actingUnit->x, GAME->actingUnit->y);
    }
    gAiTurnState->actionStep = 2;
    return;
  }
  GAME->actingUnitIndex = gUnitAction.unitIndex;
  GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
  GAME->actingUnitStartX = gUnitAction.fromX;
  GAME->actingUnitStartY = gUnitAction.fromY;
  GAME->actingUnitDestX = gUnitAction.x;
  GAME->actingUnitDestY = gUnitAction.y;
  GAME->actingUnit->flags &= ~MAP_UNIT_LOCKED;
  if ((GAME->unk_18 == 16 && gLinkPlay != 0 && Link_GetOwnPlayer() != 0) ||
      GAME->unk_18 == 21) {
    GAME->unk_46 = 0;
    GAME->actingUnit->fuel = gUnitAction.fuelLeft;
    gAiTurnState->actionStep = 1;
  }
  if (func_020b9664() == 0 || data_0229d8c4 != 4) {
    func_020f5888(GAME->actingUnit->x, GAME->actingUnit->y);
  }
  gAiTurnState->actionStep = 1;
}

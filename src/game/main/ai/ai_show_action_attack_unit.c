#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020f5888(int x, int y);

/* Display step of action 4 (attack a unit): gUnitAction.target is the
 * defender's slot. Calls func_020f5888 on its tile when unk_2338 is nonzero
 * there, restarts the frame counter and goes to step 7. */
void AiShowAction_AttackUnit(void) {
  int x = GAME->units[gUnitAction.target].x;
  int y = GAME->units[gUnitAction.target].y;

  if (GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[y] + x] != 0) {
    func_020f5888(x, y);
  }
  data_0229eac8 = 0;
  gAiTurnState->actionStep = 7;
}

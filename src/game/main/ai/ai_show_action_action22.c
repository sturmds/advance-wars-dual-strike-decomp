#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int data_0229eac8;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020f5888(s16 x, s16 y);

/* Display step of action 22 when it is not the campaign scripted case (see
 * AiShowAction_Step2): gUnitAction.target and unk_07 are a tile. Like
 * AiShowAction_AttackObject, but the steps are 5 and 9. */
void AiShowAction_Action22(void) {
  int x = gUnitAction.target;
  int y = gUnitAction.unk_07;

  if (GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[y] + x] != 0) {
    func_020f5888(x, y);
    data_0229eac8 = 0;
    gAiTurnState->actionStep = 9;
  } else {
    gAiTurnState->actionStep = 5;
  }
}

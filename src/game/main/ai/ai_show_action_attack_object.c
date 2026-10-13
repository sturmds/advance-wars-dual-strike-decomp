#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020f5888(int x, int y);

/* Display step of action 5 (attack an object): gUnitAction.target and
 * unk_07 are the object's tile. When unk_2338 is nonzero there, calls
 * func_020f5888 on the tile, zeroes the frame counter and goes to step 8;
 * otherwise goes straight to step 4. */
void AiShowAction_AttackObject(void) {
  if (GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[gUnitAction.unk_07] +
                               gUnitAction.target] != 0) {
    func_020f5888(gUnitAction.target, gUnitAction.unk_07);
    data_0229eac8 = 0;
    gAiTurnState->actionStep = 8;
  } else {
    gAiTurnState->actionStep = 4;
  }
}

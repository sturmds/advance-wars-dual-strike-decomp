#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void MapCursor_SetTile(int x, int y, s16 *tile, u16 *pixel);
void func_020b9d48(void);
void ActingUnit_FinishAction(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game_State15_RunStep sub-state 5 (UnitMoveSelect_TryMoveTo sets it when an
 * enemy blocks the path; UnloadTask_Step sets it too): puts the cursor on
 * the destination tile, ends the action there and goes back to the free
 * cursor. */
void Game_State15_EndBlockedMove(void)
{
    MapCursor_SetTile(GAME->actingUnitDestX, GAME->actingUnitDestY, &GAME->cursorX, &GAME->cursorPixelX);
    func_020b9d48();
    ActingUnit_FinishAction();
    GAME->unk_1c = 0;
}

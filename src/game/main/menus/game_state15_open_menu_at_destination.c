#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c4394(int x, int y, s16 *tile, u16 *pixel);
void UnitMenu_Open(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game_State15_RunStep sub-state 3 (set by UnitMoveSelect_TryMoveTo): puts
 * the cursor on the destination tile, calls UnitMenu_Open (guess: opens the
 * unit menu) and moves on to sub-state 4. */
void Game_State15_OpenMenuAtDestination(void)
{
    func_020c4394(GAME->actingUnitDestX, GAME->actingUnitDestY, &GAME->cursorX, &GAME->unk_2c);
    UnitMenu_Open();
    GAME->unk_1c = 4;
}

#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c4024(void);
void func_020c40d0(void);
void MapView_ScrollStep(int step);
BOOL MapView_IsAtRest(void);
u32 func_020ef1e8(void);
BOOL func_020f0190(void);
void Screen_ResetBlend(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game_State15_RunStep sub-state 8: scrolls the view until it rests and the
 * two waits have finished, then resets the blend and returns to the free cursor. */
void Game_State15_WaitScrollThenReturnToCursor(void)
{
    func_020c4024();
    func_020c40d0();
    MapView_ScrollStep(8);
    if (!MapView_IsAtRest()) {
        return;
    }
    if (!func_020ef1e8()) {
        return;
    }
    if (!func_020f0190()) {
        return;
    }
    Screen_ResetBlend();
    GAME->unk_1c = 0;
}

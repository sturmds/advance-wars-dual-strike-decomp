#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Skills_LoadPlayer1Equipped(int frontSet, int backSet);

/* Runs Skills_LoadPlayer1Equipped (sets 0 and 1) with the first screen's
 * game state current, then switches back. */
void Skills_LoadPlayer1EquippedOnMainScreen(void)
{
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedState;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Skills_LoadPlayer1Equipped(0, 1);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
}

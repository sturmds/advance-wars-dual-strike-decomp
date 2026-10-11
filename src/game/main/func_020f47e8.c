#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

BOOL func_020f4a18(void);
void SceneMode_Request(int a);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);

extern Unk_021694c0 *data_021694c0;
extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void func_020f47e8(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;

    if (func_020f4a18() == 0) {
        return;
    }
    data_021694c0->unk_10 = data_021694c0->mode;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    SceneMode_Request(9);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
}

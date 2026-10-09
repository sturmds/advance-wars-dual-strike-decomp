#include "types.h"
#include "game/force_rank_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Sound_PlaySe(int id);
void func_02041594(int a, int x, int y, int slot);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void func_02040568(void)
{
    int i;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;

    gForceRankScreen->unk_7b = 0;
    for (i = 0; i < 4; i++) {
        gForceRankScreen->unk_54[i] = 0;
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        func_02041594(1, 15 + i * 2, 256 + i * 64, gForceRankScreen->unk_54[i]);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
    }
    Sound_PlaySe(0x10009);
}

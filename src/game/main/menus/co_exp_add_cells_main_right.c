#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void func_0203220c(int x, int y);
void func_02032264(u32 x, u32 y, int matrix);
BOOL func_02038064(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* CoExp_AddCellsMainLeft with the two screens swapped. */
void CoExp_AddCellsMainRight(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_0203220c(192, 40);
    if (func_02038064()) {
        func_02032264(160, 8, 28);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    func_0203220c(64, 152);
    if (func_02038064()) {
        func_02032264(32, 120, 29);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern int data_ov000_02350530;
extern int data_ov000_02350538;
extern int data_ov000_0235053c;
extern int data_ov000_02350548;
extern int data_ov000_0235054c;
extern u8 data_ov000_023500f8[];

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void *Task_Create(const void *script, int list);
u8 Player_GetArmyStyle(int playerIndex);
BOOL func_ov000_022b0138(int unitType);
void Gfx_LoadPlayerWindowFrame(int a0);
void SecondScreen_LoadIcons(void);
void NumberFont_LoadLow(void);
void func_01ff9ac4(void);

/* Starts the second-screen unit display. The argument packs the owning
 * player (bits 5 up) and the unit type (bits 0-4). They go to
 * data_ov000_0235054c and data_ov000_02350530 (and 02350538, the shown copy),
 * the army style to 0235053c; then it creates the task with script
 * data_ov000_023500f8 on the second screen. */
void SecondScreenTask_Create(int ownerAndUnitType)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;

    data_ov000_02350530 = ownerAndUnitType & 31;
    data_ov000_02350538 = ownerAndUnitType & 31;
    data_ov000_0235054c = ownerAndUnitType >> 5;
    data_ov000_0235053c = Player_GetArmyStyle(data_ov000_0235054c);
    data_ov000_02350548 = func_ov000_022b0138(data_ov000_02350530);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Gfx_LoadPlayerWindowFrame(0);
    SecondScreen_LoadIcons();
    NumberFont_LoadLow();
    func_01ff9ac4();
    Task_Create(data_ov000_023500f8, 3);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
}

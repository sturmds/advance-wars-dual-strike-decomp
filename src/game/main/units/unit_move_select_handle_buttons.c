#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_0229eaec;

BOOL func_020d0a14(int x, int y);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020f5888(int x, int y);
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020f77bc(void);
void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Buttons while the player picks where the acting unit moves. A: tries the
 * cursor tile (func_020d0a14 counts the path). B: cancels, puts the cursor
 * back on the unit and, when data_0229eaec is set, sends unit action 19.
 * Returns TRUE when either button was handled. */
BOOL UnitMoveSelect_HandleButtons(void)
{
    u16 keys = gKeypad->pressed;

    if (keys & PAD_BUTTON_A) {
        func_020d0a14(GAME->unk_28, GAME->unk_2a);
        return TRUE;
    }
    if (keys & PAD_BUTTON_B) {
        if (data_0229eaec != 0) {
            UnitAction_Send(19, GAME->actingUnitIndex, 0, 0);
        }
        func_020f5888(GAME->actingUnit->x, GAME->actingUnit->y);
        UnitShowTask_DestroyRunning();
        func_020c8888();
        func_020f77bc();
        GAME->unk_1c = 0;
        Sound_PlaySe(0x10009);
        return TRUE;
    }
    return FALSE;
}

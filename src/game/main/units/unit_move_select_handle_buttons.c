#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"
#include "game/map_unit.h"

extern u32 gLinkPlay;

BOOL UnitMoveSelect_TryMoveTo(int x, int y);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020f5888(int x, int y);
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020f77bc(void);
void Sound_PlaySe(int se);

/* Buttons while the player picks where the acting unit moves. A: tries the
 * cursor tile (UnitMoveSelect_TryMoveTo counts the path). B: cancels, puts the cursor
 * back on the unit and, when gLinkPlay is set, sends unit action 19.
 * Returns TRUE when either button was handled. */
BOOL UnitMoveSelect_HandleButtons(void) {
  u16 keys = gKeypad->pressed;

  if (keys & PAD_BUTTON_A) {
    UnitMoveSelect_TryMoveTo(MAP_STATE->cursorX, MAP_STATE->cursorY);
    return TRUE;
  }
  if (keys & PAD_BUTTON_B) {
    if (gLinkPlay != 0) {
      UnitAction_Send(19, MAP_STATE->actingUnitIndex, 0, 0);
    }
    func_020f5888(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y);
    UnitShowTask_DestroyRunning();
    func_020c8888();
    func_020f77bc();
    MAP_STATE->unk_1c = 0;
    Sound_PlaySe(0x10009);
    return TRUE;
  }
  return FALSE;
}

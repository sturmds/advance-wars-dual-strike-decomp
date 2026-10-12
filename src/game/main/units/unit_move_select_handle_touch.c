#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/touch.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern s16 gEventScriptOwnsInput;

BOOL ActingUnit_CanMoveTo(int x, int y);
BOOL UnitMoveSelect_TryMoveTo(int x, int y);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void TouchMarker_Resume(void);
void func_020f5888(int x, int y);
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020f77bc(void);
void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* Touch version of UnitMoveSelect_HandleButtons. A release on the tile where
 * the touch began resumes the touch marker task. If ActingUnit_CanMoveTo accepts
 * that tile, the unit tries to move there (as A does); otherwise the move is
 * cancelled as B does. Returns TRUE when the touch was handled. */
BOOL UnitMoveSelect_HandleTouch(void) {
  if (gEventScriptOwnsInput != 0) {
    return FALSE;
  }
  if (!IsTouchInPhase(TOUCH_RELEASED)) {
    return FALSE;
  }
  if (GAME->cursorX == GAME->unk_30 && GAME->cursorY == GAME->unk_32) {
    TouchMarker_Resume();
    if (ActingUnit_CanMoveTo(GAME->cursorX, GAME->cursorY)) {
      UnitMoveSelect_TryMoveTo(GAME->cursorX, GAME->cursorY);
      return TRUE;
    }
    if (gLinkPlay != 0) {
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

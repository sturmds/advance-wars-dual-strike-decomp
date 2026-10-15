#include "types.h"
#include "game/Unk_02080440.h"
#include "game/touch.h"

BOOL MapTouch_InBackButton(int x, int y);
void TouchMarker_Resume(void);
void TouchMarker_Destroy(void);

extern s16 gEventScriptOwnsInput;

/* Bits returned, the same as func_020ef1bc returns for the A and B buttons. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL 0x20

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* Touch input while the player picks a map tile. A new touch on the box
 * MapTouch_InBackButton tests (x 112-144, y 0-16) cancels. A release on the tile where
 * the touch began (unk_30, unk_32, set by MapCursor_UpdateFromTouch) confirms
 * and resumes the touch marker task; TOUCH_HELD clears that tile. Returns 0
 * while gEventScriptOwnsInput is set. */
u32 MapTouch_ReadTileTapOrBack(void) {
  int touchX = gTouch.x;
  int touchY = gTouch.y;

  if (gEventScriptOwnsInput != 0) {
    return 0;
  }
  if (IsTouchInPhase(TOUCH_PRESSED) && MapTouch_InBackButton(touchX, touchY)) {
    return INPUT_CANCEL;
  }
  switch (gTouch.phase) {
    case TOUCH_PRESSED:
      break;
    case TOUCH_HELD:
      if (IsTouchInPhase(TOUCH_HELD)) {
        TouchMarker_Destroy();
        MAP_STATE->unk_30 = 0xff;
        MAP_STATE->unk_32 = 0xff;
      }
      break;
    case TOUCH_RELEASED:
      if (IsTouchInPhase(TOUCH_RELEASED)) {
        if (MAP_STATE->cursorX == MAP_STATE->unk_30 &&
            MAP_STATE->cursorY == MAP_STATE->unk_32) {
          TouchMarker_Resume();
          return INPUT_CONFIRM;
        }
      }
      break;
  }
  return 0;
}

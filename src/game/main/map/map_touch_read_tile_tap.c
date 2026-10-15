#include "types.h"
#include "game/Unk_02080440.h"
#include "game/touch.h"

void TouchMarker_Resume(void);
void TouchMarker_Destroy(void);

extern s16 gEventScriptOwnsInput;

/* Returned on a confirm, the same bit func_020ef1bc returns for the A button. */
#define INPUT_CONFIRM 0x10

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* MapTouch_ReadTileTapOrBack without the cancel box: a release on the tile
 * where the touch began confirms, TOUCH_HELD clears that tile. */
u32 MapTouch_ReadTileTap(void) {
  if (gEventScriptOwnsInput != 0) {
    return 0;
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

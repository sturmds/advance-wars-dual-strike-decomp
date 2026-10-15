#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

BOOL MapTouch_InBackButton(int x, int y);
void TouchMarker_Resume(void);
void TouchMarker_Destroy(void);
void TouchMarker_Create(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32* out);

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

/* Touch input while the player confirms the tile under the map cursor. A new
 * touch on the box MapTouch_InBackButton tests cancels. A new touch on the cursor tile
 * remembers it in unk_30 and unk_32 and calls TouchMarker_Create; one anywhere else
 * clears them. A release while they still hold the cursor tile confirms;
 * TOUCH_HELD clears them. The tile is worked out as in
 * MapCursor_UpdateFromTouch. Returns 0 while gEventScriptOwnsInput is set. */
u32 MapTouch_ReadCursorTileTap(void) {
  int touchX = gTouch.x;
  int touchY = gTouch.y;
  s32 pos[3];
  int tileX;
  int tileY;

  if (gEventScriptOwnsInput != 0) {
    return 0;
  }
  if (IsTouchInPhase(TOUCH_PRESSED) && MapTouch_InBackButton(touchX, touchY)) {
    return INPUT_CANCEL;
  }
  if (data_027c027c != data_027c0288.unk_00) {
    tileX = (touchX + MAP_STATE->unk_1c4->scrollX) >> 4;
    tileY = (touchY + MAP_STATE->unk_1c4->scrollY) >> 4;
  } else {
    Map3d_ScreenToTilePos(touchX, touchY, pos);
    pos[1] = -pos[1];
    pos[2] = 8;
    tileX = pos[0] >> 9;
    tileY = pos[1] >> 9;
  }
  if (IsTouchInPhase(TOUCH_PRESSED)) {
    if (tileX == MAP_STATE->cursorX && tileY == MAP_STATE->cursorY) {
      MAP_STATE->unk_30 = tileX;
      MAP_STATE->unk_32 = tileY;
      TouchMarker_Create(MAP_STATE->unk_30, MAP_STATE->unk_32);
    } else {
      MAP_STATE->unk_30 = 0xff;
      MAP_STATE->unk_32 = 0xff;
    }
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

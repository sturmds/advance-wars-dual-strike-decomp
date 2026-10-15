#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

void TouchMarker_Destroy(void);
void TouchMarker_Create(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32* out);
void Sound_PlaySe(int se);

extern s16 gEventScriptOwnsInput;

/* The screen is being touched and the touch began this frame. */
static inline BOOL IsTouchStarted(void) {
  if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
    return TRUE;
  }
  return FALSE;
}

/* Same code as MapCursor_UpdateFromTouch: turns the touch position into the
 * map cursor tile. A second copy in the ROM; MapCursor_UpdateFree calls it next to
 * MapCursor_MoveByKeysFree, the d-pad cursor move without the range check. */
void MapCursor_UpdateFromTouchFree(void) {
  int blocked = gEventScriptOwnsInput;
  s32 pos[3];
  int touchX = gTouch.x;
  int touchY = gTouch.y;
  int oldX = MAP_STATE->cursorX;
  int oldY = MAP_STATE->cursorY;

  if (blocked != 0) {
    return;
  }
  if (gTouch.phase == 0) {
    TouchMarker_Destroy();
    return;
  }
  if (data_027c027c != data_027c0288.unk_00) {
    MAP_STATE->cursorX = (touchX + MAP_STATE->unk_1c4->scrollX) >> 4;
    MAP_STATE->cursorY = (touchY + MAP_STATE->unk_1c4->scrollY) >> 4;
  } else {
    Map3d_ScreenToTilePos(touchX, touchY, pos);
    pos[1] = -pos[1];
    pos[2] = 8;
    MAP_STATE->cursorX = pos[0] >> 9;
    MAP_STATE->cursorY = pos[1] >> 9;
  }
  if (IsTouchStarted()) {
    MAP_STATE->unk_30 = MAP_STATE->cursorX;
    MAP_STATE->unk_32 = MAP_STATE->cursorY;
    TouchMarker_Create(MAP_STATE->unk_30, MAP_STATE->unk_32);
  }
  if (MAP_STATE->unk_30 != MAP_STATE->cursorX ||
      MAP_STATE->unk_32 != MAP_STATE->cursorY) {
    TouchMarker_Destroy();
    MAP_STATE->unk_30 = 0xff;
    MAP_STATE->unk_32 = 0xff;
  }
  if (oldX != MAP_STATE->cursorX || oldY != MAP_STATE->cursorY) {
    Sound_PlaySe(0x10000);
  }
}

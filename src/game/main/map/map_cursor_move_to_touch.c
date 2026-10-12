#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

void MapCursor_ClampToMap(void);
void Map3d_ScreenToTilePos(int x, int y, s32* out);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s16 gEventScriptOwnsInput;

#define STATE (data_027c027c->unk_48[data_027c0284])

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* Puts the map cursor on the touched tile, worked out as in
 * MapCursor_UpdateFromTouch, then runs MapCursor_ClampToMap. A touch that began
 * this frame also remembers the tile in unk_30 and unk_32. Unlike
 * MapCursor_UpdateFromTouch it plays no sound and leaves the touch marker
 * alone. */
void MapCursor_MoveToTouch(void) {
  int touchX = gTouch.x;
  int touchY = gTouch.y;
  s32 pos[3];

  if (gEventScriptOwnsInput != 0) {
    return;
  }
  if (gTouch.phase == TOUCH_RELEASED) {
    return;
  }
  if (data_027c027c != data_027c0288.unk_00) {
    STATE->cursorX = (touchX + STATE->unk_1c4->scrollX) >> 4;
    STATE->cursorY = (touchY + STATE->unk_1c4->scrollY) >> 4;
  } else {
    Map3d_ScreenToTilePos(touchX, touchY, pos);
    pos[1] = -pos[1];
    pos[2] = 8;
    STATE->cursorX = pos[0] >> 9;
    STATE->cursorY = pos[1] >> 9;
  }
  MapCursor_ClampToMap();
  if (!IsTouchInPhase(TOUCH_PRESSED)) {
    return;
  }
  STATE->unk_30 = STATE->cursorX;
  STATE->unk_32 = STATE->cursorY;
}

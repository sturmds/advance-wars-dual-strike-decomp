#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

int TargetList_FindTile(int count, int tileX, int tileY);
void TouchMarker_Destroy(void);
void TouchMarker_Create(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32* out);

extern s16 gEventScriptOwnsInput;

static inline BOOL IsTouchInPhase(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* Cursor update from touch for the target list picker in TargetListPicker_Update.
 * When the touched tile is one of the `count` tiles in the list
 * (TargetList_FindTile), the cursor (cursorX, cursorY) moves there and *index gets
 * its position in the list. The touch marker handling is the same as in
 * MapTouch_UpdateNeighborCursor. */
void MapTouch_UpdateListCursor(int* index, int count) {
  int touchX = gTouch.x;
  int touchY = gTouch.y;
  s32 pos[3];
  int tileX;
  int tileY;
  int found;

  if (gEventScriptOwnsInput != 0) {
    return;
  }
  if (gTouch.phase == TOUCH_RELEASED) {
    return;
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
  found = TargetList_FindTile(count, tileX, tileY);
  if (found >= 0) {
    MAP_STATE->cursorX = tileX;
    MAP_STATE->cursorY = tileY;
    *index = found;
  }
  if (IsTouchInPhase(TOUCH_PRESSED)) {
    MAP_STATE->unk_30 = tileX;
    MAP_STATE->unk_32 = tileY;
    TouchMarker_Create(MAP_STATE->unk_30, MAP_STATE->unk_32);
  }
  if (tileX == MAP_STATE->unk_30 && tileY == MAP_STATE->unk_32) {
    return;
  }
  TouchMarker_Destroy();
  MAP_STATE->unk_30 = 0xff;
  MAP_STATE->unk_32 = 0xff;
}

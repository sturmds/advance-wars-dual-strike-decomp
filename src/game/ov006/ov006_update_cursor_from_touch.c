#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/touch.h"

void Map3d_ScreenToTilePos(int x, int y, s32* out);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The screen is being touched and the touch began this frame. */
static inline BOOL IsTouchStarted(void) {
  if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
    return TRUE;
  }
  return FALSE;
}

/* The Design Room's copy of MapCursor_UpdateFromTouch: turns the touch in
 * touchX, touchY into a map tile (cursorX, cursorY), ignoring the top 33 pixel
 * rows. On the 3D screen the tile only changes while request is 1, and a new
 * touch only remembers its tile (unk_30, unk_32) then. No calls and no sound,
 * unlike the main version. */
void Ov006_UpdateCursorFromTouch(void) {
  s32 pos[3];
  int touchX;
  int touchY;

  if (!(data_ov006_0236d640->touch & 0x8000)) {
    return;
  }
  touchY = data_ov006_0236d640->touchY;
  touchX = data_ov006_0236d640->touchX;
  if (touchY <= 32) {
    return;
  }
  if (data_027c027c != data_027c0288.unk_00) {
    STATE->cursorX = (touchX + STATE->unk_1c4->scrollX) >> 4;
    STATE->cursorY = (touchY + STATE->unk_1c4->scrollY) >> 4;
  } else {
    Map3d_ScreenToTilePos(touchX, touchY, pos);
    pos[1] = -pos[1];
    pos[2] = 8;
    if (data_ov006_0236d640->request == 1) {
      STATE->cursorX = pos[0] >> 9;
      STATE->cursorY = pos[1] >> 9;
    }
  }
  if (IsTouchStarted() && data_ov006_0236d640->request == 1) {
    STATE->unk_30 = STATE->cursorX;
    STATE->unk_32 = STATE->cursorY;
  }
  if (STATE->unk_30 != STATE->cursorX || STATE->unk_32 != STATE->cursorY) {
    STATE->unk_30 = 0xff;
    STATE->unk_32 = 0xff;
  }
}

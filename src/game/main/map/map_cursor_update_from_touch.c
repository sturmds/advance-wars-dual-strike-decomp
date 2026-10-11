#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

void TouchMarker_Destroy(void);
void func_020f12d4(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32 *out);
void Sound_PlaySe(int se);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s16 data_0229777c;
#define STATE (data_027c027c->unk_48[data_027c0284])

/* The screen is being touched and the touch began this frame. */
static inline BOOL IsTouchStarted(void)
{
    if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
        return TRUE;
    }
    return FALSE;
}

/* Turns the touch position into a map tile (cursorX, cursorY) each frame. On the
 * screen whose map is drawn in 3D (data_027c0288.unk_00) Map3d_ScreenToTilePos projects
 * the point; on the other it is pixels plus the scroll, in 1/16 tiles. A new
 * touch remembers its tile in unk_30 and unk_32 and calls func_020f12d4; moving
 * to another tile calls TouchMarker_Destroy and clears them. The select sound plays
 * when the tile changed. */
void MapCursor_UpdateFromTouch(void)
{
    int blocked = data_0229777c;
    s32 pos[3];
    int touchX = gTouch.x;
    int touchY = gTouch.y;
    int oldX = STATE->cursorX;
    int oldY = STATE->cursorY;

    if (blocked != 0) {
        return;
    }
    if (gTouch.phase == 0) {
        TouchMarker_Destroy();
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
    if (IsTouchStarted()) {
        STATE->unk_30 = STATE->cursorX;
        STATE->unk_32 = STATE->cursorY;
        func_020f12d4(STATE->unk_30, STATE->unk_32);
    }
    if (STATE->unk_30 != STATE->cursorX || STATE->unk_32 != STATE->cursorY) {
        TouchMarker_Destroy();
        STATE->unk_30 = 0xff;
        STATE->unk_32 = 0xff;
    }
    if (oldX != STATE->cursorX || oldY != STATE->cursorY) {
        Sound_PlaySe(0x10000);
    }
}

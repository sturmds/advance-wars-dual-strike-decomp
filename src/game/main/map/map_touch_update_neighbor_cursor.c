#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

int NeighborDir_FindAtTile(int tileX, int tileY, int mask);
void TouchMarker_Destroy(void);
void func_020f12d4(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32 *out);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s16 data_0229777c;

#define GAME (data_027c027c->unk_48[data_027c0284])

static inline BOOL IsTouchInPhase(int phase)
{
    if (gTouch.active != 0 && gTouch.phase == phase) {
        return TRUE;
    }
    return FALSE;
}

/* Cursor update from touch for the four-direction tile picker in
 * NeighborTilePicker_Update. When the touched tile is one of the directions in `mask`
 * (NeighborDir_FindAtTile), the cursor (cursorX, cursorY) moves there and *direction
 * gets its index. A new touch remembers its tile in unk_30 and unk_32 and
 * starts the touch marker task; moving to another tile ends both, as in
 * MapCursor_UpdateFromTouch. */
void MapTouch_UpdateNeighborCursor(int *direction, int mask)
{
    int touchX = gTouch.x;
    int touchY = gTouch.y;
    s32 pos[3];
    int tileX;
    int tileY;
    int found;

    if (data_0229777c != 0) {
        return;
    }
    if (gTouch.phase == TOUCH_RELEASED) {
        return;
    }
    if (data_027c027c != data_027c0288.unk_00) {
        tileX = (touchX + GAME->unk_1c4->scrollX) >> 4;
        tileY = (touchY + GAME->unk_1c4->scrollY) >> 4;
    } else {
        Map3d_ScreenToTilePos(touchX, touchY, pos);
        pos[1] = -pos[1];
        pos[2] = 8;
        tileX = pos[0] >> 9;
        tileY = pos[1] >> 9;
    }
    found = NeighborDir_FindAtTile(tileX, tileY, mask);
    if (found >= 0) {
        GAME->cursorX = tileX;
        GAME->cursorY = tileY;
        *direction = found;
    }
    if (IsTouchInPhase(TOUCH_PRESSED)) {
        GAME->unk_30 = tileX;
        GAME->unk_32 = tileY;
        func_020f12d4(GAME->unk_30, GAME->unk_32);
    }
    if (tileX == GAME->unk_30 && tileY == GAME->unk_32) {
        return;
    }
    TouchMarker_Destroy();
    GAME->unk_30 = 0xff;
    GAME->unk_32 = 0xff;
}

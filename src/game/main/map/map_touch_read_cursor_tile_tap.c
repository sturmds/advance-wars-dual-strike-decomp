#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

BOOL MapTouch_InBackButton(int x, int y);
void TouchMarker_Resume(void);
void TouchMarker_Destroy(void);
void TouchMarker_Create(int tileX, int tileY);
void Map3d_ScreenToTilePos(int x, int y, s32 *out);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s16 data_0229777c;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Bits returned, the same as func_020ef1bc returns for the A and B buttons. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL  0x20

static inline BOOL IsTouchInPhase(int phase)
{
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
 * MapCursor_UpdateFromTouch. Returns 0 while data_0229777c is set. */
u32 MapTouch_ReadCursorTileTap(void)
{
    int touchX = gTouch.x;
    int touchY = gTouch.y;
    s32 pos[3];
    int tileX;
    int tileY;

    if (data_0229777c != 0) {
        return 0;
    }
    if (IsTouchInPhase(TOUCH_PRESSED) && MapTouch_InBackButton(touchX, touchY)) {
        return INPUT_CANCEL;
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
    if (IsTouchInPhase(TOUCH_PRESSED)) {
        if (tileX == GAME->cursorX && tileY == GAME->cursorY) {
            GAME->unk_30 = tileX;
            GAME->unk_32 = tileY;
            TouchMarker_Create(GAME->unk_30, GAME->unk_32);
        } else {
            GAME->unk_30 = 0xff;
            GAME->unk_32 = 0xff;
        }
    }
    switch (gTouch.phase) {
    case TOUCH_PRESSED:
        break;
    case TOUCH_HELD:
        if (IsTouchInPhase(TOUCH_HELD)) {
            TouchMarker_Destroy();
            GAME->unk_30 = 0xff;
            GAME->unk_32 = 0xff;
        }
        break;
    case TOUCH_RELEASED:
        if (IsTouchInPhase(TOUCH_RELEASED)) {
            if (GAME->cursorX == GAME->unk_30 && GAME->cursorY == GAME->unk_32) {
                TouchMarker_Resume();
                return INPUT_CONFIRM;
            }
        }
        break;
    }
    return 0;
}

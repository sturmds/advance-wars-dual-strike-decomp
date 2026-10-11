#include "types.h"
#include "game/Unk_02080440.h"
#include "game/touch.h"

BOOL MapTouch_InBackButton(int x, int y);
void func_020f1288(void);
void TouchMarker_Destroy(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
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

/* Touch input while the player picks a map tile. A new touch on the box
 * MapTouch_InBackButton tests (x 112-144, y 0-16) cancels. A release on the tile where
 * the touch began (unk_30, unk_32, set by MapCursor_UpdateFromTouch) confirms
 * and resumes the touch marker task; TOUCH_HELD clears that tile. Returns 0
 * while data_0229777c is set. */
u32 MapTouch_ReadTileTapOrBack(void)
{
    int touchX = gTouch.x;
    int touchY = gTouch.y;

    if (data_0229777c != 0) {
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
            GAME->unk_30 = 0xff;
            GAME->unk_32 = 0xff;
        }
        break;
    case TOUCH_RELEASED:
        if (IsTouchInPhase(TOUCH_RELEASED)) {
            if (GAME->cursorX == GAME->unk_30 && GAME->cursorY == GAME->unk_32) {
                func_020f1288();
                return INPUT_CONFIRM;
            }
        }
        break;
    }
    return 0;
}

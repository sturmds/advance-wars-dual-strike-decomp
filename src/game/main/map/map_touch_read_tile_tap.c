#include "types.h"
#include "game/Unk_02080440.h"
#include "game/touch.h"

void func_020f1288(void);
void TouchMarker_Destroy(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s16 data_0229777c;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Returned on a confirm, the same bit func_020ef1bc returns for the A button. */
#define INPUT_CONFIRM 0x10

static inline BOOL IsTouchInPhase(int phase)
{
    if (gTouch.active != 0 && gTouch.phase == phase) {
        return TRUE;
    }
    return FALSE;
}

/* MapTouch_ReadTileTapOrBack without the cancel box: a release on the tile
 * where the touch began confirms, TOUCH_HELD clears that tile. */
u32 MapTouch_ReadTileTap(void)
{
    if (data_0229777c != 0) {
        return 0;
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

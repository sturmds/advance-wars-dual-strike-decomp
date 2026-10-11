#include "types.h"
#include "game/Unk_02080440.h"
#include "game/touch.h"

BOOL MapCursor_SelectTile(int x, int y, int allowBuild, int allowMenu);
BOOL MapCursor_ShowRange(int x, int y);
BOOL Map_IsCoSelectPhase(void);
void func_020f1288(void);
void TouchMarker_Destroy(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s16 data_0229777c;

#define GAME (data_027c027c->unk_48[data_027c0284])

static inline BOOL IsTouchInPhase(int phase)
{
    if (gTouch.active != 0 && gTouch.phase == phase) {
        return TRUE;
    }
    return FALSE;
}

/* Touch on the map while the player moves the cursor freely. A touch held on
 * the tile where it began (unk_30, unk_32) shows the range of what stands
 * there; a release there selects the tile, as A does, unless the map is in
 * its CO select phase. Returns TRUE when either happened. */
BOOL MapTouch_HandleTileTap(void)
{
    if (data_0229777c != 0) {
        return FALSE;
    }
    switch (gTouch.phase) {
    case TOUCH_PRESSED:
        break;
    case TOUCH_HELD:
        if (IsTouchInPhase(TOUCH_HELD)) {
            if (GAME->cursorX == GAME->unk_30 && GAME->cursorY == GAME->unk_32) {
                MapCursor_ShowRange(GAME->cursorX, GAME->cursorY);
                TouchMarker_Destroy();
                return TRUE;
            }
        }
        break;
    case TOUCH_RELEASED:
        if (IsTouchInPhase(TOUCH_RELEASED)) {
            if (GAME->cursorX == GAME->unk_30 && GAME->cursorY == GAME->unk_32) {
                func_020f1288();
                if (!Map_IsCoSelectPhase()) {
                    if (MapCursor_SelectTile(GAME->cursorX, GAME->cursorY, TRUE, FALSE)) {
                        return TRUE;
                    }
                }
            }
        }
        break;
    }
    return FALSE;
}

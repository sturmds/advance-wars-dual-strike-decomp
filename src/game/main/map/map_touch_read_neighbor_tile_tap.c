#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/touch.h"

int NeighborDir_FindAtTile(int tileX, int tileY, int mask);
void TouchMarker_Resume(void);
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

/* Touch input while the player picks one of the four tiles around a center
 * tile (NeighborTilePicker_Update). The tile under the point where the touch ended is
 * worked out as in MapCursor_UpdateFromTouch. A release on the tile where the
 * touch began resumes the touch marker task, then confirms when
 * NeighborDir_FindAtTile finds that tile among the directions in `mask` and cancels
 * otherwise. */
u32 MapTouch_ReadNeighborTileTap(int mask)
{
    int endX = gTouch.endX;
    int endY = gTouch.endY;
    s32 pos[3];
    int tileX;
    int tileY;

    if (data_0229777c != 0) {
        return 0;
    }
    if (data_027c027c != data_027c0288.unk_00) {
        tileX = (endX + GAME->unk_1c4->scrollX) >> 4;
        tileY = (endY + GAME->unk_1c4->scrollY) >> 4;
    } else {
        Map3d_ScreenToTilePos(endX, endY, pos);
        pos[1] = -pos[1];
        pos[2] = 8;
        tileX = pos[0] >> 9;
        tileY = pos[1] >> 9;
    }
    if (!IsTouchInPhase(TOUCH_RELEASED)) {
        return 0;
    }
    if (tileX == GAME->unk_30 && tileY == GAME->unk_32) {
        TouchMarker_Resume();
        if (NeighborDir_FindAtTile(tileX, tileY, mask) >= 0) {
            return INPUT_CONFIRM;
        }
        return INPUT_CANCEL;
    }
    return 0;
}

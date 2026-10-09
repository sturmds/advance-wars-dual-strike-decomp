#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_022900b4;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)
/* needed to match: after a scroll store the original reloads the index first. */
#define GAME_IDX_FIRST (*(Unk_02080440_48 **)((u8 *)(data_027c0284 * sizeof(Unk_02080440_48 *)) + (u32)data_027c027c \
                                    + (u32)&((Unk_02080440 *)0)->unk_48))
#define VIEW_AGAIN (GAME_IDX_FIRST->unk_1c4)
#define STATE_50 (data_027c027c->unk_50[data_027c0284])
/* View scroll in 1/16 tiles. The header has unk_1e and unk_20 as u16; this
 * function reads and adds to them as s16 (needed to match: the right and
 * down cases use `+=` through the s16 lvalue). */
#define SCROLL_X(view) (*(s16 *)&(view)->unk_1e)
#define SCROLL_Y(view) (*(s16 *)&(view)->unk_20)

/* One frame of the map view scrolling toward the cursor tile (unk_28, unk_2a).
 * unk_2a of the view holds the scroll directions in progress: 1 right, 2 left,
 * 4 up, 8 down. A direction starts when the cursor is within 2 tiles of that
 * edge of the view, and stops at a map edge or when the scroll reaches a whole
 * tile. step is added to or taken from the scroll (in 1/16 tiles). Runs the
 * state's callback at the end. */
void MapView_ScrollStep(int step)
{
    int viewW;
    int viewH;
    int layout;
    int dist;

    layout = data_022900b4;
    viewW = data_021576fa[layout][0];
    viewH = data_021576fb[layout][0];

    if ((VIEW->unk_2a & 3) == 0) {
        dist = GAME->unk_28 - (SCROLL_X(VIEW) >> 4);
        if (dist < 2) {
            VIEW->unk_2a |= 2;
        }
        if (dist >= viewW - 2) {
            VIEW->unk_2a |= 1;
        }
    }
    if ((VIEW->unk_2a & 12) == 0) {
        dist = GAME->unk_2a - (SCROLL_Y(VIEW) >> 4);
        if (dist < 2) {
            VIEW->unk_2a |= 4;
        }
        if (dist >= viewH - 2) {
            VIEW->unk_2a |= 8;
        }
    }

    if (VIEW->unk_2a & 1) {
        if (viewW == VIEW->mapWidth - (SCROLL_X(VIEW) >> 4)) {
            VIEW->unk_2a ^= 1;
        } else {
            SCROLL_X(VIEW) += step;
            if ((SCROLL_X(VIEW_AGAIN) & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 1;
            }
        }
    }
    if (VIEW->unk_2a & 2) {
        if (SCROLL_X(VIEW) == 0) {
            VIEW->unk_2a ^= 2;
        } else {
            VIEW->unk_1e = SCROLL_X(VIEW) - step;
            if ((SCROLL_X(VIEW_AGAIN) & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 2;
            }
        }
    }
    if (VIEW->unk_2a & 4) {
        if (SCROLL_Y(VIEW) == 0) {
            VIEW->unk_2a ^= 4;
        } else {
            VIEW->unk_20 = SCROLL_Y(VIEW) - step;
            if ((SCROLL_Y(VIEW_AGAIN) & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 4;
            }
        }
    }
    if (VIEW->unk_2a & 8) {
        if (viewH == VIEW->mapHeight - (SCROLL_Y(VIEW) >> 4)) {
            VIEW->unk_2a ^= 8;
        } else {
            SCROLL_Y(VIEW) += step;
            if ((SCROLL_Y(VIEW_AGAIN) & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 8;
            }
        }
    }

    if (STATE_50->callback == NULL) {
        return;
    }
    STATE_50->callback();
}

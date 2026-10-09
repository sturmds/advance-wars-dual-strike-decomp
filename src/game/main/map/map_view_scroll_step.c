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

/* One frame of the map view scrolling toward the cursor tile (unk_28, unk_2a).
 * unk_2a of the view holds the scroll directions in progress: 1 right, 2 left,
 * 4 up, 8 down. A direction starts when the cursor is within 2 tiles of that
 * edge of the view, and stops at a map edge or when the scroll reaches a whole
 * tile. step is added to or taken from the scroll (in 1/16 tiles). Runs the
 * state's callback at the end. WIP: 94.9%, the loads of mapWidth and the scroll
 * in the right and down cases come out in the opposite order. */
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
        dist = GAME->unk_28 - ((s16)VIEW->unk_1e >> 4);
        if (dist < 2) {
            VIEW->unk_2a |= 2;
        }
        if (dist >= viewW - 2) {
            VIEW->unk_2a |= 1;
        }
    }
    if ((VIEW->unk_2a & 12) == 0) {
        dist = GAME->unk_2a - ((s16)VIEW->unk_20 >> 4);
        if (dist < 2) {
            VIEW->unk_2a |= 4;
        }
        if (dist >= viewH - 2) {
            VIEW->unk_2a |= 8;
        }
    }

    if (VIEW->unk_2a & 1) {
        if (viewW == VIEW->mapWidth - ((s16)VIEW->unk_1e >> 4)) {
            VIEW->unk_2a ^= 1;
        } else {
            VIEW->unk_1e = (s16)VIEW->unk_1e + step;
            if (((s16)VIEW_AGAIN->unk_1e & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 1;
            }
        }
    }
    if (VIEW->unk_2a & 2) {
        if ((s16)VIEW->unk_1e == 0) {
            VIEW->unk_2a ^= 2;
        } else {
            VIEW->unk_1e = (s16)VIEW->unk_1e - step;
            if (((s16)VIEW_AGAIN->unk_1e & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 2;
            }
        }
    }
    if (VIEW->unk_2a & 4) {
        if ((s16)VIEW->unk_20 == 0) {
            VIEW->unk_2a ^= 4;
        } else {
            VIEW->unk_20 = (s16)VIEW->unk_20 - step;
            if (((s16)VIEW_AGAIN->unk_20 & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 4;
            }
        }
    }
    if (VIEW->unk_2a & 8) {
        if (viewH == VIEW->mapHeight - ((s16)VIEW->unk_20 >> 4)) {
            VIEW->unk_2a ^= 8;
        } else {
            VIEW->unk_20 = (s16)VIEW->unk_20 + step;
            if (((s16)VIEW_AGAIN->unk_20 & 15) == 0) {
                VIEW_AGAIN->unk_2a ^= 8;
            }
        }
    }

    if (STATE_50->callback == NULL) {
        return;
    }
    STATE_50->callback();
}

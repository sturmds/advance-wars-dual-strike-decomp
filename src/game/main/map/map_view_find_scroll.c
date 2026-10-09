#include "types.h"
#include "game/Unk_02080440.h"

void func_020c4394(int a0, int a1, void *a2, void *a3);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_022900b4;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Works out where the map view should scroll so that tile (*x, *y) shows, and
 * stores the new top-left tile back in *x and *y. The tile is also saved as the
 * cursor tile (unk_28/unk_2c). With centre set, the view centres on the tile.
 * Otherwise it moves only when the tile is within 2 tiles of an edge (3 on the
 * right and bottom), and then puts the tile that far in from that edge. The
 * view is kept inside the map.
 * Returns TRUE if the result differs from the current scroll. The scroll
 * (unk_1e, unk_20) is in 1/16 tiles. viewW and viewH are the visible tiles for
 * the current screen screenLayout. */
BOOL MapView_FindScroll(int *x, int *y, BOOL centre)
{
    int screenLayout;
    int scrollX;
    int scrollY;
    int dx;
    u8 viewW;
    u8 viewH;
    int dy;

    screenLayout = data_022900b4;
    viewW = data_021576fa[screenLayout][0];
    viewH = data_021576fb[screenLayout][0];
    func_020c4394((s16)*x, (s16)*y, &GAME->unk_28, &GAME->unk_2c);
    scrollX = (s16)GAME->unk_1c4->unk_1e / 16;
    scrollY = (s16)GAME->unk_1c4->unk_20 / 16;
    dx = 0;
    dy = 0;
    if (centre) {
        *x -= viewW >> 1;
        *y -= viewH >> 1;
        if (*x < 0) {
            *x = 0;
        }
        if (*y < 0) {
            *y = 0;
        }
        if (*x + viewW > GAME->unk_1c4->mapWidth) {
            *x = GAME->unk_1c4->mapWidth - viewW;
        }
        if (*y + viewH > GAME->unk_1c4->mapHeight) {
            *y = GAME->unk_1c4->mapHeight - viewH;
        }
        if (scrollX == *x && scrollY == *y) {
            return 0;
        }
    } else {
        if (*x < scrollX + 2) {
            if (scrollX != 0) {
                dx = -2;
            }
        }
        if (*x > scrollX + viewW - 3) {
            if (scrollX + viewW < GAME->unk_1c4->mapWidth) {
                dx = -(viewW - 3);
            }
        }
        if (*y < scrollY + 2) {
            if (scrollY != 0) {
                dy = -2;
            }
        }
        if (*y > scrollY + viewH - 3) {
            if (scrollY + viewH < GAME->unk_1c4->mapHeight) {
                dy = -(viewH - 3);
            }
        }
        if (dx != 0) {
            *x += dx;
        } else {
            *x = scrollX;
        }
        if (dy != 0) {
            *y += dy;
        } else {
            *y = scrollY;
        }
        if (*x < 0) {
            *x = 0;
        }
        if (*y < 0) {
            *y = 0;
        }
        if (*x + viewW > GAME->unk_1c4->mapWidth) {
            *x = GAME->unk_1c4->mapWidth - viewW;
        }
        if (*y + viewH > GAME->unk_1c4->mapHeight) {
            *y = GAME->unk_1c4->mapHeight - viewH;
        }
        if (*x < 0) {
            *x = 0;
        }
        if (*y < 0) {
            *y = 0;
        }
        if (*x == scrollX && *y == scrollY) {
            return 0;
        }
    }
    return 1;
}

#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern u16 data_02297be0[];
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_0201d7d0(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define PICTURE_SIZE      6     /* the picture is 6 by 6 tiles */
#define PICTURE_TILE_BASE 0x1b3 /* first tile of the picture, stored by rows */
#define PICTURE_RIGHT_X   31    /* last column of the 32-tile map */
#define MAP_WIDTH         32
#define TILE_HFLIP        0x400

/* In the campaign, a Black Hole speaker's picture sits at the right edge, mirrored. */
#define PICTURE_ON_RIGHT (STATE->gameMode == GAME_MODE_CAMPAIGN && data_022977a4.unk_01 == ARMY_BLACK_HOLE)

static inline int Picture_GetX(void)
{
    return PICTURE_ON_RIGHT ? PICTURE_RIGHT_X : 0;
}

/* Draws the 6 by 6 tile event picture into the tile map buffer data_02297be0, shifted
 * toward the screen edge by step columns (0: fully in, 6: gone), and queues the copy
 * (func_0201d7d0). The slide holds call it once per frame. */
void EventPicture_DrawAtStep(int step)
{
    u8 row;
    u8 col;
    u16 tile;

    for (row = 0; row < PICTURE_SIZE; row++) {
        for (col = 0; col < PICTURE_SIZE; col++) {
            if (col + step >= PICTURE_SIZE) {
                tile = 0;
            } else {
                /* palette 1; needed to match: the unsigned constant keeps mwcc from folding it into the tile base */
                tile = col + PICTURE_TILE_BASE + step + row * PICTURE_SIZE + 0x1000u;
            }
            if (PICTURE_ON_RIGHT) {
                data_02297be0[row * MAP_WIDTH + (Picture_GetX() - col)] = tile | TILE_HFLIP;
            } else {
                data_02297be0[row * MAP_WIDTH + (col + Picture_GetX())] = tile;
            }
        }
    }
    func_0201d7d0();
}

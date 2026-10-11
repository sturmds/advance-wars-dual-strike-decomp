#include "types.h"
#include "game/event_picture.h"

extern u16 data_02297be0[];

void func_0201d7d0(void);

/* Draws the 6 by 6 tile event picture into the tile map buffer data_02297be0, shifted
 * toward the screen edge by step columns (0: fully in, 6: gone), and queues the copy
 * (func_0201d7d0). The slide holds call it once per frame. */
void EventPicture_DrawAtStep(int step)
{
    u8 row;
    u8 col;
    u16 tile;

    for (row = 0; row < EVENT_PICTURE_SIZE; row++) {
        for (col = 0; col < EVENT_PICTURE_SIZE; col++) {
            if (col + step >= EVENT_PICTURE_SIZE) {
                tile = 0;
            } else {
                /* needed to match: the unsigned palette constant keeps mwcc from folding it into the tile base */
                tile = col + EVENT_PICTURE_TILE_BASE + step + row * EVENT_PICTURE_SIZE + (u32)EVENT_PICTURE_PALETTE;
            }
            if (EVENT_PICTURE_ON_RIGHT) {
                data_02297be0[row * EVENT_PICTURE_MAP_WIDTH + (EventPicture_GetX() - col)] = tile | EVENT_PICTURE_HFLIP;
            } else {
                data_02297be0[row * EVENT_PICTURE_MAP_WIDTH + (col + EventPicture_GetX())] = tile;
            }
        }
    }
    func_0201d7d0();
}

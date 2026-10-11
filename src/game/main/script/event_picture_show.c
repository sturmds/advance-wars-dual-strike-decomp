#include "types.h"
#include "game/event_picture.h"

extern u16 data_02297be0[];

void EventScript_LoadCoPicture(int coMood);
void func_0201d7d0(void);

/* Draws the whole 6 by 6 event picture into the tile map buffer data_02297be0, loads the
 * graphics of CO picture coMood and queues the tile map copy (func_0201d7d0). */
void EventPicture_Show(int coMood)
{
    u16 row;
    u16 col;

    for (row = 0; row < EVENT_PICTURE_SIZE; row++) {
        for (col = 0; col < EVENT_PICTURE_SIZE; col++) {
            if (EVENT_PICTURE_ON_RIGHT) {
                data_02297be0[row * EVENT_PICTURE_MAP_WIDTH + (EventPicture_GetX() - col)] =
                    (col + EVENT_PICTURE_TILE_BASE + row * EVENT_PICTURE_SIZE)
                    | EVENT_PICTURE_PALETTE | EVENT_PICTURE_HFLIP;
            } else {
                data_02297be0[row * EVENT_PICTURE_MAP_WIDTH + (col + EventPicture_GetX())] =
                    (col + EVENT_PICTURE_TILE_BASE + row * EVENT_PICTURE_SIZE) | EVENT_PICTURE_PALETTE;
            }
        }
    }
    /* needed to match: an int parameter narrowed here */
    EventScript_LoadCoPicture((u16)coMood);
    func_0201d7d0();
}

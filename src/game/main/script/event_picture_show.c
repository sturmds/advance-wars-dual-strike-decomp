#include "types.h"
#include "game/event_picture.h"

extern u16 gEventTextMap[];

void EventScript_LoadCoPicture(int coMood);
void EventTextMap_QueueCopy(void);

/* Draws the whole 6 by 6 event picture into the tile map buffer gEventTextMap, loads the
 * graphics of CO picture coMood and queues the tile map copy (EventTextMap_QueueCopy). */
void EventPicture_Show(int coMood) {
  u16 row;
  u16 col;

  for (row = 0; row < EVENT_PICTURE_SIZE; row++) {
    for (col = 0; col < EVENT_PICTURE_SIZE; col++) {
      if (EVENT_PICTURE_ON_RIGHT) {
        gEventTextMap[row * EVENT_PICTURE_MAP_WIDTH +
                      (EventPicture_GetX() - col)] =
            (col + EVENT_PICTURE_TILE_BASE + row * EVENT_PICTURE_SIZE) |
            EVENT_PICTURE_PALETTE | EVENT_PICTURE_HFLIP;
      } else {
        gEventTextMap[row * EVENT_PICTURE_MAP_WIDTH +
                      (col + EventPicture_GetX())] =
            (col + EVENT_PICTURE_TILE_BASE + row * EVENT_PICTURE_SIZE) |
            EVENT_PICTURE_PALETTE;
      }
    }
  }
  /* needed to match: an int parameter narrowed here */
  EventScript_LoadCoPicture((u16)coMood);
  EventTextMap_QueueCopy();
}

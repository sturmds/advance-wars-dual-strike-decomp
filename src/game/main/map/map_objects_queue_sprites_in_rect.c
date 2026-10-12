#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_02168238[];
extern u8 data_02168258[];
extern u8 data_02168284[];
extern u8 data_021682a0[];
extern u8 data_021682bc[];
extern u8 data_021682d8[];

/* The original passes a fifth argument, always 0, that Map_QueueTileSprite
 * does not read. */
void Map_QueueTileSprite(int x, int y, void *data, int palette, int unused);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues a sprite at the tile of each map object whose area touches the
 * tile rectangle (left, top, width, height): volcanoes in palette 12, the
 * others in palette 8. A destroyed Black Cannon or Black Obelisk (hp 0)
 * gets data_02168284; a standing Black Cannon gets one of two sprites by
 * its facing (variant). */
void MapObjects_QueueSpritesInRect(int left, int top, int width, int height)
{
    MapObject *object = STATE->objects;
    void *sprite;

    for (; object->kind != 0; object++) {
        if (object->x + object->width < left || left + width < object->x) {
            continue;
        }
        if (object->y + object->height < top || top + height < object->y) {
            continue;
        }
        switch (object->kind) {
        case MAP_OBJECT_BLACK_CANNON:
            if (object->hp == 0) {
                sprite = data_02168284;
            } else if (object->variant == 1) {
                sprite = data_021682a0;
            } else {
                sprite = data_021682bc;
            }
            Map_QueueTileSprite(object->x, object->y, sprite, 8, 0);
            break;
        case MAP_OBJECT_BLACK_OBELISK:
            if (object->hp == 0) {
                sprite = data_02168284;
            } else {
                sprite = data_021682d8;
            }
            Map_QueueTileSprite(object->x, object->y, sprite, 8, 0);
            break;
        case MAP_OBJECT_BLOCKED:
            Map_QueueTileSprite(object->x, object->y, data_02168238, 8, 0);
            break;
        case MAP_OBJECT_VOLCANO:
            Map_QueueTileSprite(object->x, object->y, data_02168238, 12, 0);
            break;
        case 7:
            Map_QueueTileSprite(object->x, object->y, data_02168258, 8, 0);
            break;
        }
    }
}

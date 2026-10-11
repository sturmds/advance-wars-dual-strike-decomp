#include "types.h"
#include "game/map_object.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void Player_AddVision(int player);
void func_020c5674(s16 x, s16 y, int range, int gridMask, int a, int b);
void func_020c65fc(s8 *grid, int value);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Vision grid number n: the n-th 0x508-byte grid from MapTiles.inSight. */
#define VISION_GRID(n) (STATE->mapTiles->inSight + (n) * sizeof(STATE->mapTiles->inSight))

static inline BOOL Map_IsFoggy(void)
{
    BOOL foggy = TRUE;
    Unk_02080440_1c4 *info = STATE->unk_1c4;

    if (info->fog == 0 && info->weather != WEATHER_RAIN) {
        foggy = FALSE;
    }
    return foggy;
}

/* Rebuilds both vision grids (unk_20 and unk_22). Without fog or rain every
 * tile is visible. In fog or rain the grids start dark, then each player
 * 1-4 adds what its units and properties see, and every tile of a map object
 * is lit. */
void Map_RebuildVision(void)
{
    MapObject *object;
    int endX;
    int endY;
    int x;
    int y;

    func_020c65fc(VISION_GRID(STATE->unk_20), (s16)!Map_IsFoggy());
    func_020c65fc(VISION_GRID(STATE->unk_22), (s16)!Map_IsFoggy());
    if (!Map_IsFoggy()) {
        return;
    }
    Player_AddVision(1);
    Player_AddVision(2);
    Player_AddVision(3);
    Player_AddVision(4);
    for (object = STATE->objects; object->kind != 0; object++) {
        endX = object->x + object->width;
        endY = object->y + object->height;
        for (x = object->x; x < endX; x++) {
            for (y = object->y; y < endY; y++) {
                func_020c5674(x, y, 0, 3, 1, 0);
            }
        }
    }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/map_object.h"
#include "game/terrain.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_ov000_022dbd50[];
extern u16 data_ov000_022dbd52[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The map header's u16 at +0x28 and +0x2a for a map id; the header fields are
 * u8 unk_28[8] for now. */
#define HEADER_U16(symbol, mapId) (*(u16 *)((u8 *)(symbol) + (mapId) * sizeof(MapHeader)))

MapObject *Map_FindObjectCovering(int x, int y);
void func_020db3fc(void);
void func_020daaa8(int x, int y, int w, int h, int a, int b);
void func_020dab24(int x, int y);
void func_020dab8c(int x, int y, int a, int b, int owner);
void func_020dac00(int x, int y, int w, int h, int a, int b, int owner);
void func_020dac7c(int x, int y, int w, int h, int a, int b, int c);
void func_020dacf8(int x, int y, int w, int h);
void func_020dad64(int x, int y, int owner);
void func_020dadd0(int x, int y);

void Map_DrawSpecialTerrain(void)
{
    int mapA = HEADER_U16(data_ov000_022dbd50, STATE->unk_1c4->mapId);
    int mapB = HEADER_U16(data_ov000_022dbd52, STATE->unk_1c4->mapId);
    int x;
    int y;

    func_020db3fc();
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            u8 tile;

            if (Map_FindObjectCovering(x, y) != NULL) {
                continue;
            }
            tile = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];
            switch (MAP_TILE_TERRAIN(tile)) {
            case 23:
                func_020dadd0(x, y);
                break;
            case 21:
                func_020dab8c(x, y, mapA, mapB, MAP_TILE_OWNER(tile));
                break;
            case 28:
                switch (MAP_TILE_OWNER(tile)) {
                case 0:
                    func_020dac7c(x, y, 4, 4, 0, 2, 0);
                    break;
                case 1:
                    func_020dac7c(x, y, 4, 4, 2, 1, 1);
                    break;
                case 2:
                    func_020dac7c(x, y, 4, 4, 2, 1, 2);
                    break;
                }
                break;
            case 26:
                func_020dac00(x, y, 3, 3, mapA, mapB, MAP_TILE_OWNER(tile));
                break;
            case 31:
                func_020dacf8(x, y, 4, 4);
                break;
            case 24:
                func_020dad64(x, y, MAP_TILE_OWNER(tile));
                break;
            case 30:
                func_020daaa8(x, y, 3, 3, 7, 7);
                break;
            case 29:
                func_020dab24(x, y);
                break;
            }
        }
    }
}

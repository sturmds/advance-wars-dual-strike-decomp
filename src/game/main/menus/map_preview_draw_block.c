#include "types.h"
#include "game/map_data.h"
#include "game/map_header.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 gArmyColours[ARMY_SLOT_COUNT];
extern u16 data_022a6bd0[];           /* offset of each column of the block, in halfwords */
extern u16 data_022a6b80[];           /* offset of each row of the block */
extern u16 data_022a6b40[];           /* picture of a tile outside the map */
extern u16 data_02166e3c[];           /* per terrain id: first picture of the terrain in data_021587e0 */
extern u16 data_021587e0[];           /* picture number to offset into data_02149080, per army colour */
extern u16 data_02149080[];           /* pictures, 4 halfwords each, 2 halfwords apart */

/* Terrain 9 below a Black Cannon or Black Obelisk uses this picture. */
#define LARGE_OBJECT_PICTURE 31

/* Draws width by height map tiles, from tile (x, y) on, as one picture per
 * tile into dst. Used for the map preview: MapPreview_Draw calls it for
 * each block of the preview. Tiles outside the map get data_022a6b40. A
 * property takes the picture for its owner's army colour (gArmyColours). */
void MapPreview_DrawBlock(u16 *dst, int x, int y, int width, u16 height)
{
    u16 rowStart;
    int startY;
    int xEnd;
    int yEnd;
    int rowIndex;
    u16 *rowTable = data_022a6b80;
    int col;
    int row;
    int offset;
    int xx;
    int tileIndex;
    u16 *cell;

    offset = 0;
    for (col = 0; col < width; col++) {
        data_022a6bd0[col] = offset;
        if (col & 1) {
            offset += 15;
        } else {
            offset += 1;
        }
    }
    offset = 0;
    for (row = 0; row < height; row++) {
        data_022a6b80[row] = offset;
        if (row & 1) {
            offset += width * 8 - 8;
        } else {
            offset += 8;
        }
    }

    yEnd = y + height;
    rowStart = data_0229eadc->width * y;
    startY = y;
    xEnd = x + width;
    for (; y < yEnd; y++) {
        rowIndex = y - startY;
        for (xx = x, tileIndex = rowStart + x; xx < xEnd; xx++, tileIndex++) {
            u16 *picture;
            int colour;
            int terrain;
            int code;
            int tile;
            int base;

            if (xx < 0 || xx >= data_0229eadc->width || y < 0 || y >= data_0229eadc->height) {
                picture = data_022a6b40;
            } else {
                code = data_0229eadc->tileCodes[tileIndex];
                tile = data_027c027c->unk_48[data_027c0284]->tileCodeTerrain[code];
                colour = gArmyColours[tile >> 5];
                if (data_027c027c->unk_48[data_027c0284]->gameMode == 0
                    && data_027c027c->unk_48[data_027c0284]->mapTiles->unk_2338[xx + data_027c027c->unk_48[data_027c0284]->mapTiles->rowStart[y]] == 0
                    && MAP_TILE_TERRAIN(tile) != TERRAIN_HQ) {
                    colour = 0;
                }
                terrain = MAP_TILE_TERRAIN(tile);
                switch (terrain) {
                case TERRAIN_MINICANNON:
                case TERRAIN_BLACK_CANNON:
                case TERRAIN_VOLCANO:
                    base = data_02166e3c[terrain];
                    colour = 0;
                    break;
                case 9:
                    base = data_02166e3c[terrain];
                    if (y > 0) {
                        int above = MAP_TILE_TERRAIN(data_027c027c->unk_48[data_027c0284]->tileCodeTerrain[data_0229eadc->tileCodes[tileIndex - data_0229eadc->width]]);

                        if (above == TERRAIN_BLACK_CANNON || above == TERRAIN_BLACK_OBELISK) {
                            base = LARGE_OBJECT_PICTURE;
                        }
                    }
                    break;
                default:
                    base = data_02166e3c[terrain];
                    break;
                }
                picture = data_02149080 + data_021587e0[base + colour];
            }
            cell = dst + data_022a6bd0[xx - x];
            cell[rowTable[rowIndex]] = picture[0];
            cell += rowTable[rowIndex];
            cell[2] = picture[2];
            cell[4] = picture[4];
            cell[6] = picture[6];
        }
        rowStart += data_0229eadc->width;
    }
}

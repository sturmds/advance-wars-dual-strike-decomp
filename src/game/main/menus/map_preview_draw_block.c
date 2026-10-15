#include "types.h"
#include "game/map_data.h"
#include "game/map_header.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern u16 gMapPreviewColumnOffsets
    []; /* offset of each column of the block, in halfwords */
extern u16 gMapPreviewRowOffsets[]; /* offset of each row of the block */
extern u16 data_022a6b40[];         /* picture of a tile outside the map */
extern u16 data_02166e3c
    []; /* per terrain id: first picture of the terrain in data_021587e0 */
extern u16 data_021587e0
    []; /* picture number to offset into data_02149080, per army colour */
extern u16 data_02149080[]; /* pictures, 4 halfwords each, 2 halfwords apart */

/* Terrain 9 below a Black Cannon or Black Obelisk uses this picture. */
#define LARGE_OBJECT_PICTURE 31

/* Draws width by height map tiles, from tile (x, y) on, as one picture per
 * tile into dst. Used for the map preview: MapPreview_Draw calls it for
 * each block of the preview. Tiles outside the map get data_022a6b40. A
 * property takes the picture for its owner's army colour (gArmyColours).
 * Needed to match: picture, code and tile are declared at function scope,
 * the first store adds rowOff to cell in place, and the row index uses an
 * unsigned y ((u32)y - startY; found by tools/permuter.py). */
void MapPreview_DrawBlock(u16* dst, int x, int y, int width, u16 height) {
  u16 rowStart;
  /* Read through a local so the row offset is reloaded for each tile, as
     * in the original. */
  u16* rowTable = gMapPreviewRowOffsets;
  /* Column of the offset table, then the map column being drawn. */
  int col;
  int row;
  int offset;
  int startY;
  u16* cell;
  int rowOff;
  u16* picture;
  int code;
  int tile;

  offset = 0;
  for (col = 0; col < width; col++) {
    gMapPreviewColumnOffsets[col] = offset;
    if (col & 1) {
      offset += 15;
    } else {
      offset += 1;
    }
  }
  offset = 0;
  for (row = 0; row < height; row++) {
    gMapPreviewRowOffsets[row] = offset;
    if (row & 1) {
      offset += width * 8 - 8;
    } else {
      offset += 8;
    }
  }

  rowStart = data_0229eadc->width * y;
  startY = y;
  for (; y < startY + height; y++) {
    for (col = x; col < x + width; col++) {
      int colour;
      int terrain;
      int base;

      if (col < 0 || col >= data_0229eadc->width || y < 0 ||
          y >= data_0229eadc->height) {
        picture = data_022a6b40;
      } else {
        code = data_0229eadc->tileCodes[rowStart + col];
        tile = data_027c027c->unk_48[data_027c0284]->tileCodeTerrain[code];
        colour = gArmyColours[tile >> 5];
        if (data_027c027c->unk_48[data_027c0284]->gameMode ==
                GAME_MODE_CAMPAIGN &&
            data_027c027c->unk_48[data_027c0284]
                    ->mapTiles
                    ->unk_2338[col + data_027c027c->unk_48[data_027c0284]
                                         ->mapTiles->rowStart[y]] == 0 &&
            MAP_TILE_TERRAIN(tile) != TERRAIN_HQ) {
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
          case TERRAIN_OBJECT_PART:
            base = data_02166e3c[terrain];
            if (y > 0) {
              int above = MAP_TILE_TERRAIN(
                  data_027c027c->unk_48[data_027c0284]->tileCodeTerrain
                      [data_0229eadc
                           ->tileCodes[rowStart + col - data_0229eadc->width]]);

              if (above == TERRAIN_BLACK_CANNON ||
                  above == TERRAIN_BLACK_OBELISK) {
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
      cell = dst + gMapPreviewColumnOffsets[col - x];
      rowOff = rowTable[(u32)y - startY];
      *(cell += rowOff) = picture[0];
      cell[2] = picture[2];
      cell[4] = picture[4];
      cell[6] = picture[6];
    }
    rowStart += data_0229eadc->width;
  }
}

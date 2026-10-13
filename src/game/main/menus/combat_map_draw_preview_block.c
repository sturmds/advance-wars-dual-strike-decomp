#include "types.h"
#include "game/map_data.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern u16 gMapPreviewColumnOffsets
    []; /* offset of each column of the block, in halfwords */
extern u16 gMapPreviewRowOffsets[]; /* offset of each row of the block */
extern u16 data_022a6b40[];         /* picture of a tile outside the map */
extern u16 data_02166e3c
    []; /* per terrain id: first picture of the terrain in data_021587e0 */
extern u16 data_021587e0[]; /* picture number to offset into data_02149080 */
extern u16 data_02149080[]; /* pictures, 4 halfwords each, 2 halfwords apart */

/* Picture used for Minicannons and Black Cannons. */
#define CANNON_PICTURE 25

/* Like MapPreview_DrawBlock, for a map that is not loaded into the game
 * state: draws width by height tiles of map, from tile (x, y) on, as one
 * 4x4-pixel picture per tile into dst. codeTerrain maps a tile code to its
 * terrain id (bits 0-4) and colour (bits 5-7).
 * Needed to match: row is also the counter of the row offset loop, and
 * cell is set in one sum. Each alone leaves the row counter in another
 * register than lr. */
void CombatMap_DrawPreviewBlock(const MapData* map, const u8* codeTerrain,
                                u16* dst, s16 x, s16 y, u16 width, u16 height) {
  int col;
  int row;
  int offset;
  u16 rowStart;
  u16* picture;
  int tile;
  int terrain;
  int colour;
  int base;
  u16* cell;
  /* Read through a local so the row offset is reloaded for each tile, as
     * in MapPreview_DrawBlock. */
  u16* rowTable = gMapPreviewRowOffsets;

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

  rowStart = map->width * y;
  for (row = y; row < y + height; row++) {
    for (col = x; col < x + width; col++) {
      if (col < 0 || col >= map->width || row < 0 || row >= map->height) {
        picture = data_022a6b40;
      } else {
        tile = codeTerrain[map->tileCodes[rowStart + col]];
        colour = (tile >> 5) & 7;
        terrain = MAP_TILE_TERRAIN(tile);
        switch (terrain) {
          case TERRAIN_MINICANNON:
          case TERRAIN_BLACK_CANNON:
            base = CANNON_PICTURE;
            colour = 0;
            break;
          case 25:
            base = 0;
            colour = 0;
            break;
          default:
            base = data_02166e3c[terrain];
            break;
        }
        picture = data_02149080 + data_021587e0[base + colour];
      }
      cell = dst + gMapPreviewColumnOffsets[col - x] + rowTable[row - y];
      cell[0] = picture[0];
      cell[2] = picture[2];
      cell[4] = picture[4];
      cell[6] = picture[6];
    }
    rowStart += map->width;
  }
}

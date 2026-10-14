#include "types.h"
#include "game/map_data.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/map_preview.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gSavedArmyColours[5];
u8 gHasBackupMemory;
int data_0229b058;
int data_0229b054;
extern u16 gMapPreviewColumnOffsets[];
extern u16 gMapPreviewRowOffsets[];
extern u16 data_022a6b40[];
extern u16 data_02166e3c[];
/* "09f" */
char data_021587dc[4] = {
#include "data/main/data_021587dc.inc.c"
};
/* "0a0" */
char data_021587d8[4] = {
#include "data/main/data_021587d8.inc.c"
};
u16 data_021587e0[32] = {
#include "data/main/data_021587e0.inc.c"
};
extern u16 data_02149080[];

/* Draws the lower half of the map preview into dst: rows from half the
 * map height (rounded up to even) to the end, and columns up to the width
 * rounded up to even, one picture per tile. Same drawing as
 * MapPreview_DrawBlock, with gSavedArmyColours for property owners, the
 * neutral colour check gated by data_0229b054 instead of the game mode, and
 * data_0229b058 as the running index of the row's first tile. Terrain ids
 * 24 and 30 have no names yet; MapPreview_DrawBlock tests 26 and 29 in the
 * same places. */
void MapPreview_DrawLowerHalf(u16* dst) {
  int tile;
  int base;
  u8 colour;
  int col;
  u16* picture;
  int terrain;
  u16* cell;
  u8 above;
  u16* rowTable;
  int row;

  /* Needed to match: the row offset is read through a local, once per tile. */
  rowTable = gMapPreviewRowOffsets;

  for (row = ((data_0229eadc->height + 1) & ~1) >> 1;
       row < ((data_0229eadc->height + 1) & ~1); row++) {
    for (col = 0; col < ((data_0229eadc->width + 1) & ~1); col++) {
      if (col >= data_0229eadc->width || row >= data_0229eadc->height) {
        picture = data_022a6b40;
      } else {
        tile = data_027c027c->unk_48[data_027c0284]->tileCodeTerrain
                   [data_0229eadc->tileCodes[data_0229b058 + col]];
        colour = gSavedArmyColours[tile >> 5];
        if (data_0229b054 == 0 &&
            data_027c027c->unk_48[data_027c0284]
                    ->mapTiles
                    ->unk_2338[col + data_027c027c->unk_48[data_027c0284]
                                         ->mapTiles->rowStart[row]] == 0 &&
            MAP_TILE_TERRAIN(tile) != TERRAIN_HQ) {
          colour = 0;
        }
        terrain = MAP_TILE_TERRAIN(tile);
        if (terrain == TERRAIN_MINICANNON ||
            terrain == TERRAIN_GRAND_BOLT_WEAK_POINT ||
            terrain == TERRAIN_VOLCANO) {
          colour = 0;
        }
        if (terrain == TERRAIN_OBJECT_PART && row > 0) {
          above =
              data_027c027c->unk_48[data_027c0284]->tileCodeTerrain
                  [data_0229eadc
                       ->tileCodes[data_0229b058 + col - data_0229eadc->width]];
          if (above == TERRAIN_BLACK_CANNON || above == TERRAIN_DEATHRAY) {
            base = 31;
          } else {
            base = data_02166e3c[terrain];
          }
        } else {
          base = data_02166e3c[terrain];
        }
        picture = data_02149080 + data_021587e0[base + colour];
      }
      cell = dst + gMapPreviewColumnOffsets[col] + rowTable[row];
      cell[0] = picture[0];
      cell[2] = picture[2];
      cell[4] = picture[4];
      cell[6] = picture[6];
    }
    data_0229b058 += data_0229eadc->width;
  }
}

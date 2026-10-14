#include "types.h"
#include "game/map_header.h"
#include "game/map_data.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/map_preview.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gSavedArmyColours[];
static int gMapPreviewWidth;
static int gMapPreviewHeight;
static int gMapPreviewColumnSteps[2];
static int gMapPreviewRowSteps[2];
extern u16 gMapPreviewColumnOffsets[];
extern u16 gMapPreviewRowOffsets[];
extern u16 data_022a6b40[];
extern u16 data_02166e3c[];
extern u16 data_021587e0[];
extern u16 data_02149080[];
extern u8 MainLoop_MapFrame[];

void* func_01ff9c38(void);

/* Sets up the map preview and draws its upper half into dst. The size
 * rounded up to even goes to gMapPreviewWidth and gMapPreviewHeight. The
 * column offsets (gMapPreviewColumnOffsets) step by 1 and 15 in turn, the row offsets
 * (gMapPreviewRowOffsets) by 8 and width * 8 - 8. data_0229b054 is 0 only while the main loop is
 * MainLoop_MapFrame; MapPreview_DrawLowerHalf then draws the rest. The tile
 * drawing is the same as there. */
void MapPreview_DrawUpperHalf(u16* dst) {
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
  int rows;
  int offset;
  int width;

  if (func_01ff9c38() == MainLoop_MapFrame) {
    data_0229b054 = 0;
  } else {
    data_0229b054 = 1;
  }
  width = (data_0229eadc->width + 1) & ~1;
  gMapPreviewWidth = width;
  gMapPreviewHeight = (data_0229eadc->height + 1) & ~1;
  gMapPreviewColumnSteps[0] = 1;
  gMapPreviewColumnSteps[1] = 15;
  gMapPreviewRowSteps[0] = 8;
  gMapPreviewRowSteps[1] = width * 8 - 8;
  for (offset = 0, col = 0; col < width; col++) {
    gMapPreviewColumnOffsets[col] = offset;
    offset += gMapPreviewColumnSteps[col & 1];
  }
  for (offset = 0, row = 0; row < gMapPreviewHeight; row++) {
    gMapPreviewRowOffsets[row] = offset;
    offset += gMapPreviewRowSteps[row & 1];
  }
  data_0229b058 = 0;

  /* Needed to match: the row offset is read through a local, once per tile. */
  rowTable = gMapPreviewRowOffsets;
  rows = ((data_0229eadc->height + 1) & ~1) >> 1;
  for (row = 0; row < rows; row++) {
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

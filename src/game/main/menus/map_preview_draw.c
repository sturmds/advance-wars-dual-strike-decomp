#include "types.h"
#include "game/map_data.h"
#include "game/map_header.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

u32 File_UnpackTo(void* dest, const char* path);
const char* File_LocalizePath(const char* path);
void Map_LoadData(u32 mapId);
void Map_LoadArmyColoursById(u32 mapId);
void MapPreview_DrawBlock(void* dst, s16 x, s16 y, int width, int height);
void func_020c3e5c(void);

extern const char data_02159964[];
extern u8 gMapPreviewPropertyCounts[6];
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PREVIEW_BLOCK_SIZE 0x800

/* Indexes of gMapPreviewPropertyCounts: how many of each property the map has. */
enum {
  COUNT_BASE,
  COUNT_AIRPORT,
  COUNT_PORT,
  COUNT_CITY,
  COUNT_COM_TOWER,
  COUNT_SILO,
  COUNT_KINDS
};

/* Draws a preview of map mapId into dst, 9 blocks of 16 by 16 tiles (the
 * last row 8 tiles high) cut from the middle of the map, and counts its
 * properties in gMapPreviewPropertyCounts. The player-made maps use a stored picture
 * instead and are not counted. */
void MapPreview_Draw(u8* dst, int mapId) {
  /* Needed to match: x and y hold the block origin first, then serve as
     * loop counters. */
  int x;
  int y;
  u16 rowStart;

  if (MAP_IS_CUSTOM(mapId)) {
    File_UnpackTo(dst, File_LocalizePath(data_02159964));
    return;
  }
  Map_LoadData((u16)mapId);
  Map_LoadArmyColoursById(mapId);

  x = (data_0229eadc->width - 48) >> 1;
  y = (data_0229eadc->height - 40) >> 1;
  MapPreview_DrawBlock(dst, x, y, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE, x + 16, y, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 2, x + 32, y, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 3, x, y + 16, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 4, x + 16, y + 16, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 5, x + 32, y + 16, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6, x, y + 32, 16, 8);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6 + 0x400, x + 16, y + 32, 16,
                       8);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6 + 0x800, x + 32, y + 32, 16,
                       8);

  rowStart = 0;
  for (x = 0; x < COUNT_KINDS; x++) {
    gMapPreviewPropertyCounts[x] = 0;
  }
  for (y = 0; y < data_0229eadc->height; y++) {
    for (x = 0; x < data_0229eadc->width; x++) {
      switch (MAP_TILE_TERRAIN(
          data_027c027c->unk_48[data_027c0284]
              ->tileCodeTerrain[data_0229eadc->tileCodes[rowStart + x]])) {
        case TERRAIN_CITY:
          gMapPreviewPropertyCounts[COUNT_CITY]++;
          break;
        case TERRAIN_BASE:
          gMapPreviewPropertyCounts[COUNT_BASE]++;
          break;
        case TERRAIN_AIRPORT:
          gMapPreviewPropertyCounts[COUNT_AIRPORT]++;
          break;
        case TERRAIN_PORT:
          gMapPreviewPropertyCounts[COUNT_PORT]++;
          break;
        case TERRAIN_SILO:
          gMapPreviewPropertyCounts[COUNT_SILO]++;
          break;
        case TERRAIN_COM_TOWER:
          gMapPreviewPropertyCounts[COUNT_COM_TOWER]++;
          break;
      }
    }
    rowStart += data_0229eadc->width;
  }
  func_020c3e5c();
}

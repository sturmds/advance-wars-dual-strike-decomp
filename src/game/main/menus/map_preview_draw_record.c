#include "types.h"
#include "game/design_map.h"
#include "game/map_data.h"
#include "game/map_header.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Map_SetDataBuffer(void* buffer);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);
void MapPreview_DrawBlock(void* dst, s16 x, s16 y, int width, int height);
void func_020c3e5c(void);

/* also declared in src/game/main/map/map_load_army_colours.c and 3 more */
u8 gArmyColours[ARMY_SLOT_COUNT];

#define PREVIEW_BLOCK_SIZE 0x800

/* Draws a preview of the player-made map in record into dst, the way
 * MapPreview_Draw does for a map id: 9 blocks of 16 by 16 tiles (the last
 * row 8 tiles high) cut from the middle of the map. A record with no map
 * clears dst instead. The record's army colours replace gArmyColours. */
void MapPreview_DrawRecord(u8* dst, DesignMapRecord* record) {
  int slot;
  int originX;
  int originY;

  if (record->playerCount == 0xff || record->playerCount == 0) {
    Mem_Zero32(0, dst, 0x4000);
    return;
  }
  for (slot = 0; slot < ARMY_SLOT_COUNT; slot++) {
    gArmyColours[slot] = record->armyColours[slot];
  }
  Map_SetDataBuffer(Mem_Alloc(0x724));
  Mem_CopyUnits(record, data_0229eadc, 0x724, 8);

  originX = (data_0229eadc->width - 48) >> 1;
  originY = (data_0229eadc->height - 40) >> 1;
  MapPreview_DrawBlock(dst, originX, originY, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE, originX + 16, originY, 16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 2, originX + 32, originY, 16,
                       16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 3, originX, originY + 16, 16,
                       16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 4, originX + 16, originY + 16,
                       16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 5, originX + 32, originY + 16,
                       16, 16);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6, originX, originY + 32, 16,
                       8);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6 + 0x400, originX + 16,
                       originY + 32, 16, 8);
  MapPreview_DrawBlock(dst + PREVIEW_BLOCK_SIZE * 6 + 0x800, originX + 32,
                       originY + 32, 16, 8);
  func_020c3e5c();
}

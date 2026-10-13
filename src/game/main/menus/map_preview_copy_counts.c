#include "types.h"

void Mem_CopyUnits(const void* src, void* dst, int size, int unitBits);
/* also declared in src/game/main/menus/map_preview_draw.c */
u8 gMapPreviewPropertyCounts[0x6];

/* Copies the 6 property counts (bases, airports, ports, cities, Com Towers,
 * silos) that the last MapPreview_Draw call left into dst. */
void MapPreview_CopyCounts(void* dst) {
  Mem_CopyUnits(gMapPreviewPropertyCounts, dst, 6, 8);
}

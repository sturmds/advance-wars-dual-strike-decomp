#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/map_data.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern const char data_021612f8[]; /* "/combat/res_combat_pnl" */
extern const char data_02161310[]; /* "/combat" */
extern const char data_02161318[]; /* "%03x" */
extern const u8 data_02160adc[];   /* file numbers of the combat maps */

void* Mem_Alloc(u32 size);
void Mem_Free(void* ptr);
void File_UnpackTo(void* dest, const char* fileName);
void FS_ChangeDir(const char* dir);
int sprintf(char* dest, const char* format, ...);
void CombatMap_DrawPreviewBlock(const MapData* map, const u8* codeTerrain,
                                u16* dst, s16 x, s16 y, u16 width, u16 height);

/* OBJ tile where the preview of combat map i starts; each preview takes
 * 0x100 tiles, as four 64x64-pixel quarters of 0x40 tiles. */
#define PREVIEW_TILE(i, quarter) (0x400 + (i) * 0x100 + (quarter) * 0x40)

/* Loads the COMBAT_MAP_COUNT maps of Combat mode and draws the middle 32x32 tiles of each
 * as a map preview into OBJ VRAM, one 16x16-tile quarter at a time. */
void CombatSetup_LoadMapPreviews(void) {
  MapData* map;
  u8* codeTerrain;
  int i;
  char path[12];

  map = Mem_Alloc(sizeof(CombatMapBuffer));
  codeTerrain = Mem_Alloc(MAP_TILE_CODE_COUNT);
  File_UnpackTo(codeTerrain, data_021612f8);
  FS_ChangeDir(data_02161310);
  for (i = 0; i < COMBAT_MAP_COUNT; i++) {
    sprintf(path, data_02161318, data_02160adc[i]);
    File_UnpackTo(map, path);
    CombatMap_DrawPreviewBlock(
        map, codeTerrain,
        (u16*)(data_027c0280->objVram + PREVIEW_TILE(i, 0) * 32),
        (map->width >> 1) - 16, (map->height >> 1) - 16, 16, 16);
    CombatMap_DrawPreviewBlock(
        map, codeTerrain,
        (u16*)(data_027c0280->objVram + PREVIEW_TILE(i, 1) * 32),
        map->width >> 1, (map->height >> 1) - 16, 16, 16);
    CombatMap_DrawPreviewBlock(
        map, codeTerrain,
        (u16*)(data_027c0280->objVram + PREVIEW_TILE(i, 2) * 32),
        (map->width >> 1) - 16, map->height >> 1, 16, 16);
    CombatMap_DrawPreviewBlock(
        map, codeTerrain,
        (u16*)(data_027c0280->objVram + PREVIEW_TILE(i, 3) * 32),
        map->width >> 1, map->height >> 1, 16, 16);
  }
  Mem_Free(map);
  Mem_Free(codeTerrain);
}

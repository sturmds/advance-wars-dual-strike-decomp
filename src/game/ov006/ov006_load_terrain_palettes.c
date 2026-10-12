#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/terrain.h"

const void* Terrain_GetPalette(int id, int army);
void Palette_Load(const void* src, u32 offset, u32 size);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_027c0288 data_027c0288;
extern Unk_027c0280* data_027c0280;
extern Unk_02080440* data_027c027c;

/* Loads seven terrain palettes of 32 bytes into the main screen's palette
 * buffer: neutral plain, mountain, sea, road, pipe and missile silo from
 * offset 0x320, and a player 1 lab at 0x300. */
void Ov006_LoadTerrainPalettes(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Palette_Load(Terrain_GetPalette(TERRAIN_PLAIN, 0), 0x320, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_MOUNTAIN, 0), 0x340, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_SEA, 0), 0x360, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_ROAD, 0), 0x380, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_PIPE, 0), 0x3a0, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_SILO, 0), 0x3c0, 32);
  Palette_Load(Terrain_GetPalette(TERRAIN_LAB, 1), 0x300, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

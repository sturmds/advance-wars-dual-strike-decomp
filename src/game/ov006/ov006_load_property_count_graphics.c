#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void func_0201e6e8(int x, int y, int width, int height);
void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);
const void* Terrain_GetPalette(int id, int army);
u8* UnitIcons_GetTiles(void);
int Player_GetUnitIconTile(int player, int unitType);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void MenuBox_Draw(int x, int y, int width, int height);

extern const u16 gArmyPalettes[][16];

/* Loads the sub screen sprites of the count table that
 * Ov006_DrawPropertyCounts draws: eight terrain sprites (Palette_QueueBlockCopy) at
 * 0x3000-0x3700 in OBJ VRAM, the CO icons of players 1-4 at 0x3c00, the
 * players' army colours as OBJ palettes 11-14, and the property palettes
 * of armies 0-4 (id 20) as OBJ palettes 4-8, which the table rows use. Also
 * fills a 16 x 20 block of layer 1 at (16, 3) (func_0201e6e8) and draws a
 * menu box. */
void Ov006_LoadPropertyCountGraphics(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_0201e6e8(16, 3, 16, 20);

  Palette_QueueBlockCopy(6, 0, data_027c0280->objVram + 0x3000, 1);
  Palette_QueueBlockCopy(14, 1, data_027c0280->objVram + 0x3100, 1);
  Palette_QueueBlockCopy(10, 0, data_027c0280->objVram + 0x3200, 1);
  Palette_QueueBlockCopy(11, 0, data_027c0280->objVram + 0x3300, 1);
  Palette_QueueBlockCopy(22, 0, data_027c0280->objVram + 0x3400, 1);
  Palette_QueueBlockCopy(14, 0, data_027c0280->objVram + 0x3500, 1);
  Palette_QueueBlockCopy(17, 0, data_027c0280->objVram + 0x3600, 1);
  Palette_QueueBlockCopy(16, 0, data_027c0280->objVram + 0x3700, 1);

  TransferQueue_Add(UnitIcons_GetTiles() + Player_GetUnitIconTile(1, 2) * 32,
                    data_027c0280->objVram + 0x3c00, 128);
  TransferQueue_Add(UnitIcons_GetTiles() + Player_GetUnitIconTile(2, 2) * 32,
                    data_027c0280->objVram + 0x3c80, 128);
  TransferQueue_Add(UnitIcons_GetTiles() + Player_GetUnitIconTile(3, 2) * 32,
                    data_027c0280->objVram + 0x3d00, 128);
  TransferQueue_Add(UnitIcons_GetTiles() + Player_GetUnitIconTile(4, 2) * 32,
                    data_027c0280->objVram + 0x3d80, 128);

  Palette_Load(gArmyPalettes[MAP_STATE->players[1].armyColour - 1], 0x360, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[2].armyColour - 1], 0x380, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[3].armyColour - 1], 0x3a0, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[4].armyColour - 1], 0x3c0, 32);

  Palette_Load(Terrain_GetPalette(20, 0), 0x280, 32);
  Palette_Load(Terrain_GetPalette(20, 1), 0x2a0, 32);
  Palette_Load(Terrain_GetPalette(20, 2), 0x2c0, 32);
  Palette_Load(Terrain_GetPalette(20, 3), 0x2e0, 32);
  Palette_Load(Terrain_GetPalette(20, 4), 0x300, 32);
  Palette_Load(Terrain_GetPalette(17, 0), 0x320, 32);
  Palette_Load(Terrain_GetPalette(15, 0), 0x340, 32);

  MenuBox_Draw(3, 2, 10, 4);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

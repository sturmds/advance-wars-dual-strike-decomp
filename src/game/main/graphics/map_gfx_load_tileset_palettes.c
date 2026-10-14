#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_tileset_files.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 data_0216826c
    []; /* per army colour (and player slot): byte offset of its colours in the tileset palette */

void Palette_Load(const void* src, u16 offset, u32 size);
void MapGfx_LoadTilesetObjPalette(int weather, int tileset, int row);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads the tileset's three BG palette rows from row on. Then in each
 * player slot's colours, four colours (5, 7, 9 and 11 from its offset) take
 * the values of the player's army colour. Ends with the tileset's OBJ
 * palette in OBJ row 8. */
void MapGfx_LoadTilesetPalettes(int weather, int tileset, int row) {
  const u8* palette;
  const u8* src;
  int dst;
  int i;
  row <<= 5; /* needed to match: row now holds the byte offset */
  palette = data_02167e30[tileset].bgPalettes;

  Palette_Load(palette, row, 0x60);
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    src = palette + data_0216826c[STATE->players[i].armyColour];
    dst = row + data_0216826c[i];
    Palette_Load(src + 10, dst + 10, 2);
    Palette_Load(src + 14, dst + 14, 2);
    Palette_Load(src + 18, dst + 18, 2);
    Palette_Load(src + 22, dst + 22, 2);
  }
  MapGfx_LoadTilesetObjPalette(weather, tileset, 8);
}

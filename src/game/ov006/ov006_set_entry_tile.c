#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);
const void* Terrain_GetIconTiles(int id);
const void* Terrain_GetPalette(int id, int army);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern Unk_027c0288 data_027c0288;
extern Unk_027c0280* data_027c0280;
extern Unk_02080440* data_027c027c;

/* Picks map tile byte `tile` as the one shown in the entry view. For a
 * city, HQ, airport, port, base or com tower the army becomes the tile's
 * owner (0 neutral, 1-4); for other tiles it stays. Then loads the tiles
 * (sub screen object VRAM at 0x5280 and 0x5380) and the palette (offset
 * 0x260) of the terrain for that army, like Ov006_LoadEntryTerrainGraphics. */
void Ov006_SetEntryTile(int tile) {
  int army = data_ov006_0236d640->tileArmy;
  int terrain;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch (tile) {
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_COM_TOWER:
      army = 0;
      break;
    case MAP_TILE(TERRAIN_CITY, 1):
    case MAP_TILE(TERRAIN_HQ, 1):
    case MAP_TILE(TERRAIN_AIRPORT, 1):
    case MAP_TILE(TERRAIN_PORT, 1):
    case MAP_TILE(TERRAIN_BASE, 1):
    case MAP_TILE(TERRAIN_COM_TOWER, 1):
      army = 1;
      break;
    case MAP_TILE(TERRAIN_CITY, 2):
    case MAP_TILE(TERRAIN_HQ, 2):
    case MAP_TILE(TERRAIN_AIRPORT, 2):
    case MAP_TILE(TERRAIN_PORT, 2):
    case MAP_TILE(TERRAIN_BASE, 2):
    case MAP_TILE(TERRAIN_COM_TOWER, 2):
      army = 2;
      break;
    case MAP_TILE(TERRAIN_CITY, 3):
    case MAP_TILE(TERRAIN_HQ, 3):
    case MAP_TILE(TERRAIN_AIRPORT, 3):
    case MAP_TILE(TERRAIN_PORT, 3):
    case MAP_TILE(TERRAIN_BASE, 3):
    case MAP_TILE(TERRAIN_COM_TOWER, 3):
      army = 3;
      break;
    case MAP_TILE(TERRAIN_CITY, 4):
    case MAP_TILE(TERRAIN_HQ, 4):
    case MAP_TILE(TERRAIN_AIRPORT, 4):
    case MAP_TILE(TERRAIN_PORT, 4):
    case MAP_TILE(TERRAIN_BASE, 4):
    case MAP_TILE(TERRAIN_COM_TOWER, 4):
      army = 4;
      break;
  }
  data_ov006_0236d640->tileArmy = army;
  data_ov006_0236d640->toolTile = tile;
  terrain = MAP_TILE_TERRAIN(tile);

  Palette_QueueBlockCopy(
      terrain, army, ((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5280,
      1);
  TransferQueue_Add(Terrain_GetIconTiles(terrain),
                    ((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5380,
                    0x100);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Palette_Load(Terrain_GetPalette(terrain, army), 0x260, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

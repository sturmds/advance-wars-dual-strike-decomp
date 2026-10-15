#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern u8 Ov006_gSlotArmyColours[]; /* 1, 2, 3, 4 */
/* Map cell code of each property kind, by owner 1-4. */
extern u16 Ov006_gComTowerCells[];
extern u16 Ov006_gAirportCells[];
extern u16 Ov006_gCityCells[];
extern u16 Ov006_gBaseCells[];
extern u16 Ov006_gHqCells[];
extern u16 Ov006_gPortCells[];
/* Per old owner 1-4: [restore][hqMask] gives the new owner times 2. Row 1
 * of each (data_ov006_0236d578 and so on) undoes row 0. */
extern u8 data_ov006_0236d568[][16];
extern u8 data_ov006_0236d578[];
extern u8 data_ov006_0236d588[][16];
extern u8 data_ov006_0236d598[];
extern u8 data_ov006_0236d5a8[][16];
extern u8 data_ov006_0236d5b8[];
extern u8 data_ov006_0236d5c8[][16];
extern u8 data_ov006_0236d5d8[];

int Ov006_CountArmyFactories(
    int tile); /* guess: finds a tile with this byte; below 0 when there is none */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))

/* Gives tile x, y the owner *owner / 2 and, for a property, the matching
 * cell code. */
static inline void SetTileOwner(int x, int y, int tile, const u8* owner) {
  int gfx;

  tile = (tile & 31) | (*owner << 4);
  MAP_STATE->mapTiles->tiles[TILE_INDEX(x, y)] = tile;
  switch (tile & 31) {
    case TERRAIN_HQ:
      gfx = Ov006_gHqCells[(*owner - 2) >> 1];
      break;
    case TERRAIN_CITY:
      gfx = Ov006_gCityCells[(*owner - 2) >> 1];
      break;
    case TERRAIN_BASE:
      gfx = Ov006_gBaseCells[(*owner - 2) >> 1];
      break;
    case TERRAIN_AIRPORT:
      gfx = Ov006_gAirportCells[(*owner - 2) >> 1];
      break;
    case TERRAIN_PORT:
      gfx = Ov006_gPortCells[(*owner - 2) >> 1];
      break;
    case TERRAIN_COM_TOWER:
      gfx = Ov006_gComTowerCells[(*owner - 2) >> 1];
      break;
    default:
      gfx = -1;
      break;
  }
  if (gfx >= 0) {
    MAP_STATE->mapTiles->mapCells[TILE_INDEX(x, y)] = gfx;
  }
}

/* Design Room: with restore 0, moves the armies that have an HQ to the
 * first player slots, keeping their order, and returns which slots had one
 * (8 player 1 ... 1 player 4). Owned tiles and property cells follow their
 * army, and each army keeps its colour. Restore 1 with the returned mask
 * puts the armies back and resets each army colour to its slot number.
 * The callers pack, pass the map to DesignMap_SaveToSlot (likely the save), then
 * restore. */
int Ov006_PackArmySlots(int restore, int hqMask) {
  int tile;
  int x;
  int y;

  if (restore == 0) {
    hqMask = 0;
    /* HQ tile bytes of owners 1-4 */
    hqMask |= (Ov006_CountArmyFactories(0x28) >= 0) << 3;
    hqMask |= (Ov006_CountArmyFactories(0x48) >= 0) << 2;
    hqMask |= (Ov006_CountArmyFactories(0x68) >= 0) << 1;
    hqMask |= (Ov006_CountArmyFactories(0x88) >= 0);
    MAP_STATE->army[1] =
        Ov006_gSlotArmyColours[(data_ov006_0236d578[hqMask] >> 1) - 1];
    MAP_STATE->army[2] =
        Ov006_gSlotArmyColours[(data_ov006_0236d598[hqMask] >> 1) - 1];
    MAP_STATE->army[3] =
        Ov006_gSlotArmyColours[(data_ov006_0236d5b8[hqMask] >> 1) - 1];
    MAP_STATE->army[4] =
        Ov006_gSlotArmyColours[(data_ov006_0236d5d8[hqMask] >> 1) - 1];
  } else {
    MAP_STATE->players[1].armyColour = 1;
    MAP_STATE->players[2].armyColour = 2;
    MAP_STATE->players[3].armyColour = 3;
    MAP_STATE->players[4].armyColour = 4;
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      tile = MAP_STATE->mapTiles->tiles[TILE_INDEX(x, y)];
      switch (tile & 0xe0) {
        case 0x20:
          SetTileOwner(x, y, tile, &data_ov006_0236d568[restore][hqMask]);
          break;
        case 0x40:
          SetTileOwner(x, y, tile, &data_ov006_0236d588[restore][hqMask]);
          break;
        case 0x60:
          SetTileOwner(x, y, tile, &data_ov006_0236d5a8[restore][hqMask]);
          break;
        case 0x80:
          SetTileOwner(x, y, tile, &data_ov006_0236d5c8[restore][hqMask]);
          break;
      }
    }
  }
  return hqMask;
}

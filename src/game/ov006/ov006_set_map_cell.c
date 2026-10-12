#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[STATE->mapTiles->rowStart[y] + (x)])

/* Writes map cell code cell at tile x, y, and for the codes listed here also
 * the tile's terrain and owner (players 1 and 2 only). Other codes leave the
 * tile byte as it was. */
void Ov006_SetMapCell(int x, int y, int cell) {
  STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + x] = cell;
  switch (cell) {
    case 1:
      TILE_AT(x, y) = TERRAIN_PLAIN;
      break;
    case 32:
      TILE_AT(x, y) = TERRAIN_MOUNTAIN;
      break;
    case 135:
      TILE_AT(x, y) = TERRAIN_WOOD;
      break;
    case 97:
      TILE_AT(x, y) = TERRAIN_ROAD;
      break;
    case 19:
      TILE_AT(x, y) = TERRAIN_BRIDGE;
      break;
    case 42:
      TILE_AT(x, y) = TERRAIN_SEA;
      break;
    case 0:
      TILE_AT(x, y) = TERRAIN_SHOAL;
      break;
    case 360:
      TILE_AT(x, y) = TERRAIN_REEF;
      break;
    case 450:
      TILE_AT(x, y) = TERRAIN_CITY;
      break;
    case 455:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_CITY, 1);
      break;
    case 460:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_CITY, 2);
      break;
    case 454:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_BASE, 1);
      break;
    case 459:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_BASE, 2);
      break;
    case 456:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_AIRPORT, 1);
      break;
    case 461:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_AIRPORT, 2);
      break;
    case 457:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_PORT, 1);
      break;
    case 462:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_PORT, 2);
      break;
    case 453:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_HQ, 1);
      break;
    case 458:
      TILE_AT(x, y) = MAP_TILE(TERRAIN_HQ, 2);
      break;
    case 441:
      TILE_AT(x, y) = TERRAIN_COM_TOWER;
      break;
    case 22:
      TILE_AT(x, y) = TERRAIN_BRIDGE;
      break;
    case 21:
      TILE_AT(x, y) = TERRAIN_BRIDGE;
      break;
  }
}

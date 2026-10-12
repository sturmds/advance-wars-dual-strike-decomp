#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* TRUE when the terrain of a map tile byte (low 5 bits) is an HQ, base,
 * airport, port or communication tower. */
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile) {
  switch (MAP_TILE_TERRAIN(tile)) {
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_COM_TOWER:
      return TRUE;
  }
  return FALSE;
}

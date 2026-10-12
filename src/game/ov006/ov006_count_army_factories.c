#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

BOOL Ov006_GetHqPosition(int hqTile, int* x,
                         int* y); /* the army's HQ position, if it has one */
int Ov006_CountPropertyTilesMatching(int tile);

/* Number of bases, airports and ports that army owns, or -1 when the army
 * has no HQ on the map. hqTile is the army's HQ tile byte (terrain and
 * owner, MapTiles.tiles). */
int Ov006_CountArmyFactories(int hqTile) {
  int x;
  int y;
  int count;

  if (!Ov006_GetHqPosition(hqTile, &x, &y)) {
    return -1;
  }
  count = 0;
  switch (hqTile) {
    case MAP_TILE_MAKE(TERRAIN_HQ, 1):
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_BASE, 1));
      count +=
          Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_AIRPORT, 1));
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_PORT, 1));
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 2):
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_BASE, 2));
      count +=
          Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_AIRPORT, 2));
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_PORT, 2));
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 3):
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_BASE, 3));
      count +=
          Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_AIRPORT, 3));
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_PORT, 3));
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 4):
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_BASE, 4));
      count +=
          Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_AIRPORT, 4));
      count += Ov006_CountPropertyTilesMatching(MAP_TILE_MAKE(TERRAIN_PORT, 4));
      break;
  }
  return count;
}

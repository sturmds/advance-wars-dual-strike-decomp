#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_CountMatchingNeighbors(int x, int y);
void Ov006_TurnToSea(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
int Ov006_GetShoalGraphic(int x, int y);
void Ov006_StoreMapCell(int x, int y, int value);
int Ov006_TileMatches(int x, int y, int terrain);
void Ov006_RefreshNeighborShoals(int x, int y);

/* Tries to put shoal at (x, y). 0: refused (Ov006_CountMatchingNeighbors or
 * Ov006_GetShoalGraphic is negative, and the old tile is put back in the second
 * case); 1: placed. */
int Ov006_PlaceShoal(int x, int y) {
  u8 oldTile;
  int result;

  if (Ov006_CountMatchingNeighbors(x, y) < 0) {
    return 0;
  }
  Ov006_TurnToSea(x, y);
  oldTile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];
  Ov006_SetTerrain(x, y, TERRAIN_SHOAL);
  result = Ov006_GetShoalGraphic(x, y);
  if (result < 0) {
    Ov006_SetTerrain(x, y, oldTile);
    return 0;
  }
  Ov006_StoreMapCell(x, y, result);
  if (Ov006_TileMatches(x, y, TERRAIN_SEA)) {
    return 1;
  }
  Ov006_RefreshNeighborShoals(x, y);
  return 1;
}

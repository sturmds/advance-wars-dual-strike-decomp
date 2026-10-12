#include "types.h"
#include "game/terrain.h"

int Ov006_TileMatches(int x, int y, int terrain);
void Ov006_SetTerrain(int x, int y, int terrain);
int Ov006_IsSurroundedByWater(int x, int y);

/* A reef tile at (x, y) that is not surrounded by sea or reef becomes sea. */
void Ov006_ReefToSeaIfNotOpenWater(int x, int y) {
  if (!Ov006_TileMatches(x, y, TERRAIN_REEF)) {
    return;
  }
  if (Ov006_IsSurroundedByWater(x, y)) {
    return;
  }
  Ov006_SetTerrain(x, y, TERRAIN_SEA);
}

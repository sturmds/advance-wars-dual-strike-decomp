#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

void Ov006_RemoveUnitOnImpassableTile(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Puts terrain at tile x, y. Sea also runs Ov006_RemoveUnitOnImpassableTile for the tile. */
void Ov006_SetTerrain(int x, int y, int terrain) {
  MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;

  map->tiles[map->rowStart[y] + x] = terrain;
  if (terrain == TERRAIN_SEA) {
    Ov006_RemoveUnitOnImpassableTile(x, y);
  }
}

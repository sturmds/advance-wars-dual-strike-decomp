#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_GetWaterCell(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_PlaceRiver(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void func_ov006_0235da70(int x, int y, int arg);
int Ov006_GetRoadCell(int x, int y);
int Ov006_GetCellMatchedToLeft(int x, int y);
void func_ov006_02363198(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TERRAIN_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

#define CELL_SEA 42 /* the cell Ov006_FillMapForScreen uses for sea */

/* Picks the cell of tile x, y again after a tile next to it changed in the
 * editor. Ov006_GetWaterCell gives the new cell (negative: none; sea then
 * gets the plain sea cell). Rivers keep their cell here; with refitRivers,
 * the four river tiles around x, y are refit instead. Plain, road, mountain
 * and wood tiles then get the cell their own fit functions choose. */
void Ov006_RefitTile(int x, int y, int refitRivers) {
  int terrain = TERRAIN_AT(x, y);
  int cell = Ov006_GetWaterCell(x, y);

  if (cell < 0 && terrain == TERRAIN_SEA) {
    cell = CELL_SEA;
  }
  if (terrain != TERRAIN_RIVER) {
    if (cell == CELL_SEA) {
      Ov006_SetTerrain(x, y, TERRAIN_SEA);
    }
    Ov006_StoreMapCell(x, y, cell);
  }
  if (refitRivers) {
    if (TERRAIN_AT(x, y - 1) == TERRAIN_RIVER) {
      Ov006_PlaceRiver(x, y - 1);
    }
    if (TERRAIN_AT(x, y + 1) == TERRAIN_RIVER) {
      Ov006_PlaceRiver(x, y + 1);
    }
    if (TERRAIN_AT(x - 1, y) == TERRAIN_RIVER) {
      Ov006_PlaceRiver(x - 1, y);
    }
    if (TERRAIN_AT(x + 1, y) == TERRAIN_RIVER) {
      Ov006_PlaceRiver(x + 1, y);
    }
  }
  Ov006_ResetNeighborsToSea(x, y);
  if (terrain == TERRAIN_PLAIN) {
    func_ov006_0235da70(x, y, 1);
  } else if (terrain == TERRAIN_ROAD) {
    Ov006_StoreMapCell(x, y, Ov006_GetRoadCell(x, y));
    Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
  } else if (terrain == TERRAIN_MOUNTAIN) {
    func_ov006_02363198(x, y);
  } else if (terrain == TERRAIN_WOOD) {
    Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
  }
}

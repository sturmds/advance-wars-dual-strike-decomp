#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_GetWaterCell(int x, int y);
void Ov006_StoreMapCell(int x, int y, int value);
void Ov006_SetTerrain(int x, int y, int terrain);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Stores the cell Ov006_GetWaterCell picks for tile x, y: 42 (sea) when it
 * returns 0, or when it returns a negative value on a sea tile. A cell of 42
 * also sets the terrain to sea. */
void Ov006_FitWaterCell(int x, int y) {
  int cell = Ov006_GetWaterCell(x, y);

  if (cell == 0) {
    cell = 42;
  } else if (cell < 0) {
    if (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] ==
        TERRAIN_SEA) {
      cell = 42;
    }
  }
  Ov006_StoreMapCell(x, y, cell);
  if (cell == 42) {
    Ov006_SetTerrain(x, y, TERRAIN_SEA);
  }
}

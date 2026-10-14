#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

BOOL Ov006_IsNotSeaTile(int x,
                        int y); /* FALSE when the tile is sea, shoal or reef */
BOOL Ov006_CountNeighborsOnSide(
    int x, int y, int side); /* side: 0 left, 1 right, 2 up, 4 down */
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_PlaceShoal(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Map cell codes. The river cell names are guesses from which neighbours
 * each one checks; each mouth cell names the side of the river it sits on. */
#define CELL_RIVER_HORIZONTAL 57
#define CELL_RIVER_VERTICAL 24
#define CELL_MOUTH_LEFT 0x11d
#define CELL_MOUTH_RIGHT 0xfd
#define CELL_MOUTH_UP 0xfc
#define CELL_MOUTH_DOWN 0x11c

/* Map editor: when the river cell at x, y meets sea, shoal or reef on a side
 * it runs to, turns that tile into a river mouth (terrain river with a mouth
 * cell). If a shoal is next to the new mouth, Ov006_PlaceShoal refits it.
 * The shoal checks use the tile at x - 1 for the up and down mouths and
 * row y - 1 for the left and right mouths, as the original does. */
void Ov006_AddRiverMouths(int x, int y) {
  int cell = CELL_AT(x, y);

  if (cell == CELL_RIVER_HORIZONTAL) {
    if (x > 0 && !Ov006_IsNotSeaTile(x - 1, y) &&
        CELL_AT(x, y) == CELL_RIVER_HORIZONTAL &&
        !Ov006_CountNeighborsOnSide(x - 1, y, 0)) {
      Ov006_SetTerrain(x - 1, y, TERRAIN_RIVER);
      Ov006_StoreMapCell(x - 1, y, CELL_MOUTH_LEFT);
      if (TILE_AT(x - 1, y - 1) == TERRAIN_SHOAL) {
        Ov006_PlaceShoal(x - 1, y - 1);
      }
    }
    if (x < STATE->unk_1c4->mapWidth - 1 && !Ov006_IsNotSeaTile(x + 1, y) &&
        CELL_AT(x, y) == CELL_RIVER_HORIZONTAL &&
        !Ov006_CountNeighborsOnSide(x + 1, y, 1)) {
      Ov006_SetTerrain(x + 1, y, TERRAIN_RIVER);
      Ov006_StoreMapCell(x + 1, y, CELL_MOUTH_RIGHT);
      if (TILE_AT(x + 1, y - 1) == TERRAIN_SHOAL) {
        Ov006_PlaceShoal(x + 1, y - 1);
      }
    }
  }
  if (cell != CELL_RIVER_VERTICAL) {
    return;
  }
  if (y > 0 && !Ov006_IsNotSeaTile(x, y - 1) &&
      CELL_AT(x, y) == CELL_RIVER_VERTICAL &&
      !Ov006_CountNeighborsOnSide(x, y - 1, 2)) {
    Ov006_SetTerrain(x, y - 1, TERRAIN_RIVER);
    Ov006_StoreMapCell(x, y - 1, CELL_MOUTH_UP);
    if (TILE_AT(x - 1, y - 1) == TERRAIN_SHOAL) {
      Ov006_PlaceShoal(x - 1, y - 1);
    }
  }
  if (y < STATE->unk_1c4->mapHeight - 1 && !Ov006_IsNotSeaTile(x, y + 1) &&
      CELL_AT(x, y) == CELL_RIVER_VERTICAL &&
      !Ov006_CountNeighborsOnSide(x, y + 1, 4)) {
    Ov006_SetTerrain(x, y + 1, TERRAIN_RIVER);
    Ov006_StoreMapCell(x, y + 1, CELL_MOUTH_DOWN);
    if (TILE_AT(x - 1, y + 1) == TERRAIN_SHOAL) {
      Ov006_PlaceShoal(x - 1, y + 1);
    }
  }
}

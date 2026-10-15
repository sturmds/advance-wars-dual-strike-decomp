#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"

void Units_ClearAll(void);
void func_020c4448(void);
void Ov006_NewMap(void);
void Ov006_SetTerrain(int x, int y, int terrain);
void func_ov006_02363198(int x, int y);
void Ov006_FindHqs(void);
u8 Ov006_CountPropertyTiles(void);

/* Sets up the map for ov006 screen screenId (negative: Ov006_NewMap
 * does it). Builds the row start table, then sets every tile: terrain 1
 * (plain), 4 (wood) or 7 (sea) for screens 1, 4 and 7, and for screen 3 mountain
 * with an extra call per tile. The tile store as (tiles + x)[row] is needed to
 * match. */
void Ov006_FillMapForScreen(int screenId) {
  int i;
  int y;
  int x;

  if (screenId < 0) {
    Ov006_NewMap();
    return;
  }
  Units_ClearAll();
  data_ov006_0236d640->unk_2c[0] = 0;
  for (i = 0; i < MAP_STATE->unk_1c4->mapHeight; i++) {
    MAP_STATE->mapTiles->rowStart[i] = i * MAP_STATE->unk_1c4->mapWidth;
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      switch (screenId) {
        case 7:
          MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] =
              42;
          (MAP_STATE->mapTiles->tiles + x)[MAP_STATE->mapTiles->rowStart[y]] =
              TERRAIN_SEA;
          break;
        case 1:
          MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] =
              1;
          (MAP_STATE->mapTiles->tiles + x)[MAP_STATE->mapTiles->rowStart[y]] =
              TERRAIN_PLAIN;
          break;
        case 3:
          Ov006_SetTerrain(x, y, TERRAIN_MOUNTAIN);
          func_ov006_02363198(x, y);
          break;
        case 4:
          MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] =
              135;
          (MAP_STATE->mapTiles->tiles + x)[MAP_STATE->mapTiles->rowStart[y]] =
              TERRAIN_WOOD;
          break;
      }
    }
  }
  func_020c4448();
  Ov006_FindHqs();
  data_ov006_0236d640->propertyCount = Ov006_CountPropertyTiles();
}

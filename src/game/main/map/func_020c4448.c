#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/terrain.h"

void Map_MarkCrystalObeliskRange(void);

extern s8 Ov000_gUrbanTerrain[];
extern s8 data_ov000_022f4578[];

/* Scans the map once for property tiles (urban terrain, plus silos): counts them
 * per owner in Player.propertyCount, numbers them in MapTiles.unk_1928 and lists them in
 * unk_6088. A second scan lists up to 92 tiles in unk_608c (pipe seams count only in
 * gameMode 4).
 * Needed to match: i holds the urban flag in the first scan, count is reused for the
 * second list, and the terrain is recomputed from the tile at each use. */
void func_020c4448(void) {
  int tile;
  int i;
  int count;
  int col;
  int x;
  int y;
  int row;

  count = 0;
  RangeMap_SetBuffer(&MAP_STATE->mapTiles->unk_1928);
  RangeMap_Fill(-1);
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    MAP_STATE->players[i].propertyCount = 0;
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];
      i = Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(tile)];
      if (i != 0) {
        MAP_STATE->players[MAP_TILE_OWNER(tile)].propertyCount++;
      }
      if (i != 0 || MAP_TILE_TERRAIN(tile) == TERRAIN_SILO) {
        MAP_STATE->mapTiles->unk_1928[x + MAP_STATE->mapTiles->rowStart[y]] =
            count;
        MAP_STATE->unk_6088[count].terrain = MAP_TILE_TERRAIN(tile);
        MAP_STATE->unk_6088[count].x = x;
        MAP_STATE->unk_6088[count].y = y;
        MAP_STATE->unk_6088[count].claimCount = 0;
        count++;
      }
    }
  }
  MAP_STATE->unk_6088[count].terrain = 0xff;
  count = 0;
  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
      tile =
          MAP_STATE->mapTiles->tiles[col + MAP_STATE->mapTiles->rowStart[row]];
      if (data_ov000_022f4578[MAP_TILE_TERRAIN(tile)] != 0 ||
          (MAP_STATE->gameMode == 4 &&
           MAP_TILE_TERRAIN(tile) == TERRAIN_PIPE_SEAM)) {
        if (count < 92) {
          MAP_STATE->unk_608c[count].tile = tile;
          MAP_STATE->unk_608c[count].x = col;
          MAP_STATE->unk_608c[count].y = row;
        }
        count++;
      }
    }
  }
  if (count < 92) {
    MAP_STATE->unk_608c[count].tile = 0xff;
  }
  Map_MarkCrystalObeliskRange();
}

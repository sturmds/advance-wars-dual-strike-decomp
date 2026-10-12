#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

void Map_LoadIntoState(int mapId);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gUrbanTerrain[32];
extern u8 data_022a6c9d[][27];
extern u8 data_022a6c9e[][27];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets *total to the number of property tiles (urban terrain) on the map and
 * *most to the largest count held by one owner slot, plus 1 when that owner
 * does not hold them all. Custom maps read both from a table. Other maps are
 * loaded first (Map_LoadIntoState), since the tiles are read from the loaded
 * state. BattleSetup_LoadFromGame stores the results in the battle setup. */
void Map_LoadAndCountProperties(int mapId, u8* most, u8* total) {
  u8 counts[6];
  int i;
  int x;
  int y;
  u8 tile;

  if (MAP_IS_CUSTOM(mapId)) {
    *total = data_022a6c9d[mapId - MAP_CUSTOM_FIRST][0];
    *most = data_022a6c9e[mapId - MAP_CUSTOM_FIRST][0];
    return;
  }
  Map_LoadIntoState(mapId);
  for (i = 0; i < 6; i++) {
    counts[i] = 0;
  }
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      tile = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];
      if (Ov000_gUrbanTerrain[tile & 0x1f] != 0) {
        counts[5]++;
        counts[(tile & 0xe0) >> 5]++;
      }
    }
  }
  counts[0] = 0;
  for (i = 0; i < 5; i++) {
    if (counts[0] < counts[i]) {
      counts[0] = counts[i];
    }
  }
  *total = counts[5];
  if (counts[0] < counts[5]) {
    counts[0]++;
  }
  *most = counts[0];
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

void Map_LoadIntoState(int mapId);

extern s8 Ov000_gUrbanTerrain[];
extern u8 data_022a6c9d[][27];
extern u8 data_022a6c9e[][27];

/* Sets *total to the number of property tiles (urban terrain) on the map and
 * *most to the largest count held by one owner slot, plus 1 when that owner
 * does not hold them all. Custom maps read both from a table. Other maps are
 * loaded first (Map_LoadIntoState), since the tiles are read from the loaded
 * state. BattleSetup_LoadFromGame stores the results in the battle setup. */
void Map_LoadAndCountProperties(int mapId, u8* most, u8* total) {
  u8 counts[ARMY_SLOT_COUNT + 1]; /* per owner slot, then the total */
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
  for (i = 0; i < (int)ARRAY_COUNT(counts); i++) {
    counts[i] = 0;
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];
      if (Ov000_gUrbanTerrain[tile & 0x1f] != 0) {
        counts[ARMY_SLOT_COUNT]++;
        counts[(tile & 0xe0) >> 5]++;
      }
    }
  }
  counts[0] = 0;
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    if (counts[0] < counts[i]) {
      counts[0] = counts[i];
    }
  }
  *total = counts[ARMY_SLOT_COUNT];
  if (counts[0] < counts[ARMY_SLOT_COUNT]) {
    counts[0]++;
  }
  *most = counts[0];
}

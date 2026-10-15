#include "types.h"
#include "game/Unk_02080440.h"

void func_020c4f20(void);
void Players_AssignVisionGrids(void);
void func_020c4018(void);
void Gfx_LoadPlayerBmap0a0Palette(int player);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Run after a battle save is restored: converts every map cell code back to
 * its terrain and owner byte, then rebuilds what depends on the map
 * (func_020c4f20, the screen config's unk_54 hook, vision grids,
 * func_020c4018) and loads the current player's palette. */
void Map_RefreshAfterRestore(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      int tile = x + MAP_STATE->mapTiles->rowStart[y];
      MAP_STATE->mapTiles->tiles[tile] =
          MAP_STATE->tileCodeTerrain[MAP_STATE->mapTiles->mapCells[tile]];
    }
  }
  func_020c4f20();
  if (HOOKS->unk_54 != NULL) {
    HOOKS->unk_54(1);
  }
  Players_AssignVisionGrids();
  func_020c4018();
  Gfx_LoadPlayerBmap0a0Palette(MAP_STATE->currentPlayer);
}

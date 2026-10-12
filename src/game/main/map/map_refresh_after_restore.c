#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_020c4f20(void);
void Players_AssignVisionGrids(void);
void func_020c4018(void);
void Gfx_LoadPlayerBmap0a0Palette(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Run after a battle save is restored: converts every map cell code back to
 * its terrain and owner byte, then rebuilds what depends on the map
 * (func_020c4f20, the screen config's unk_54 hook, vision grids,
 * func_020c4018) and loads the current player's palette. */
void Map_RefreshAfterRestore(void) {
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      int tile = x + STATE->mapTiles->rowStart[y];
      STATE->mapTiles->tiles[tile] =
          STATE->tileCodeTerrain[STATE->mapTiles->mapCells[tile]];
    }
  }
  func_020c4f20();
  if (HOOKS->unk_54 != NULL) {
    HOOKS->unk_54(1);
  }
  Players_AssignVisionGrids();
  func_020c4018();
  Gfx_LoadPlayerBmap0a0Palette(STATE->currentPlayer);
}

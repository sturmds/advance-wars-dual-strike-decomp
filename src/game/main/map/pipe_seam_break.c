#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/terrain.h"

void TileEffectTask_Create(int x, int y, int effect, Task* parent);
void func_020c4018(void);
void func_020c4448(void);

/* Map cell codes of the two pipe seams and of the broken seams they become. */
#define CELL_PIPE_SEAM_A 0x162
#define CELL_PIPE_SEAM_B 0x163
#define CELL_BROKEN_PIPE_SEAM_A 0x122
#define CELL_BROKEN_PIPE_SEAM_B 0x123

/* TileEffectTask effect of a hit on a pipe seam; MapHitTask_ShowResult plays it on
 * every hit that leaves the seam standing too */
#define EFFECT_PIPE_SEAM_HIT 28

/* Breaks the pipe seam at tile (x, y): plays the effect under `parent`, turns
 * the tile into plain and swaps its graphic for the broken seam. */
void PipeSeam_Break(int x, int y, Task* parent) {
  u16 cell = MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(x, y)];

  TileEffectTask_Create(x, y, EFFECT_PIPE_SEAM_HIT, parent);
  if (cell == CELL_PIPE_SEAM_A) {
    MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] = TERRAIN_PLAIN;
    MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(x, y)] =
        CELL_BROKEN_PIPE_SEAM_A;
  }
  if (cell == CELL_PIPE_SEAM_B) {
    MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] = TERRAIN_PLAIN;
    MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(x, y)] =
        CELL_BROKEN_PIPE_SEAM_B;
  }
  func_020c4018();
  func_020c4448();
}

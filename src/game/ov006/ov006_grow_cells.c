#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_gen.h"

int Ov006_CountCellNeighbors(int x, int y, int cell);
BOOL Ov006_RollCellChance(int count, int cell);
void Ov006_SetMapCell(int x, int y, int cell);
BOOL Ov006_SettleSeedTiles(void);

/* Ov006_gGenMarks marks (see Ov006_SettleSeedTiles). */
#define MARK_CLEAR 0
#define MARK_DONE 1
#define MARK_SEED 2

/* Grows the patches of `cell` over tiles of fromCell, in rounds: each
 * unmarked fromCell tile rolls Ov006_RollCellChance on how many of its
 * neighbours hold `cell`, and turns into `cell` as a new seed if it passes or
 * is marked done if not. Stops after a round that adds nothing. */
void Ov006_GrowCells(int cell, int fromCell) {
  int x;
  int y;

  do {
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (Ov006_gGenMarks[y][x] == MARK_CLEAR &&
            fromCell == MAP_STATE->mapTiles
                            ->mapCells[MAP_STATE->mapTiles->rowStart[y] + x]) {
          if (Ov006_RollCellChance(Ov006_CountCellNeighbors(x, y, cell),
                                   cell) == TRUE) {
            Ov006_SetMapCell(x, y, cell);
            Ov006_gGenMarks[y][x] = MARK_SEED;
          } else {
            Ov006_gGenMarks[y][x] = MARK_DONE;
          }
        }
      }
    }
  } while (Ov006_SettleSeedTiles());
}

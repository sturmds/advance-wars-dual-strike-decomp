#include "types.h"
#include "game/design_map_edit.h"
#include "game/Unk_02080440.h"

int Ov006_GetPipeLink(
    int x, int y,
    int dir); /* guess: how tile x, y joins its neighbour in direction dir (0: not at all) */
int Ov006_FitPipeCell(int x, int y,
                      int flag); /* guess: picks the cell code for tile x, y */
int Ov006_GetPipeEndCell(int x,
                         int y); /* guess: picks the cell code for tile x, y */
void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern s16 Ov006_gNeighborDx[]; /* left, right, up, down */
extern s16 Ov006_gNeighborDy[];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->unk_1c4)
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

#define NX (x + Ov006_gNeighborDx[i])
#define NY (y + Ov006_gNeighborDy[i])

/* True when cell is first or first + 1. Needed to match: the original
 * subtracts through a u16 constant (cell + 0x10000 - first). */
#define CELL_IS_PAIR(cell, first) ((u16)((cell) + (u16) - (first)) <= 1)

/* The cell codes Ov006_GetPipeLink joins: 0x102-0x103, 0x120-0x123,
 * 0x140-0x143 and 0x160-0x163 (rows 8 to 11 of the 32-wide cell set). */
#define IS_JOINED_CELL(cell)                                                   \
  ((cell) == 0x142 || (cell) == 0x143 || (cell) == 0x140 || (cell) == 0x141 || \
   (cell) == 0x160 || (cell) == 0x161 || (cell) == 0x162 || (cell) == 0x163 || \
   (cell) == 0x122 || (cell) == 0x123 || (cell) == 0x121 || (cell) == 0x120 || \
   CELL_IS_PAIR(cell, 0x102))

/* Refits the joined cells around tile x, y after an edit there, in three
 * passes over the four neighbours on the map. A neighbour that x, y joins
 * with kind 2 or 3, or whose cell is 0x162 or 0x163, gets a new cell code.
 * Between passes, x, y itself is refitted when it holds a joined cell.
 * The first pass and the first refit pick with Ov006_FitPipeCell, the
 * rest with Ov006_GetPipeEndCell. */
void Ov006_RefitJoinedCellsAround(int x, int y) {
  int i;
  int cell;

  for (i = 0; i < DESIGN_DIR_COUNT; i++) {
    if (NX < 0 || NX >= MAP->mapWidth || NY < 0 || NY >= MAP->mapHeight) {
      continue;
    }
    if (Ov006_GetPipeLink(x, y, i) != 0 &&
        (Ov006_GetPipeLink(x, y, i) == 2 || Ov006_GetPipeLink(x, y, i) == 3)) {
      Ov006_StoreMapCell(NX, NY, Ov006_FitPipeCell(NX, NY, 0));
    } else if (CELL_IS_PAIR(CELL_AT(NX, NY), 0x162)) {
      Ov006_StoreMapCell(NX, NY, Ov006_FitPipeCell(NX, NY, 0));
    }
  }
  cell = CELL_AT(x, y);
  if (IS_JOINED_CELL(cell)) {
    Ov006_StoreMapCell(x, y, Ov006_FitPipeCell(x, y, 0));
  }

  for (i = 0; i < DESIGN_DIR_COUNT; i++) {
    if (NX < 0 || NX >= MAP->mapWidth || NY < 0 || NY >= MAP->mapHeight) {
      continue;
    }
    if (Ov006_GetPipeLink(x, y, i) != 0 &&
        (Ov006_GetPipeLink(x, y, i) == 2 || Ov006_GetPipeLink(x, y, i) == 3)) {
      Ov006_StoreMapCell(NX, NY, Ov006_FitPipeCell(NX, NY, 0));
    } else if (CELL_IS_PAIR(CELL_AT(NX, NY), 0x162)) {
      Ov006_StoreMapCell(NX, NY, Ov006_FitPipeCell(NX, NY, 0));
    }
  }
  cell = CELL_AT(x, y);
  if (IS_JOINED_CELL(cell)) {
    Ov006_StoreMapCell(x, y, Ov006_GetPipeEndCell(x, y));
  }

  for (i = 0; i < DESIGN_DIR_COUNT; i++) {
    if (NX < 0 || NX >= MAP->mapWidth || NY < 0 || NY >= MAP->mapHeight) {
      continue;
    }
    if (Ov006_GetPipeLink(x, y, i) != 0 &&
        (Ov006_GetPipeLink(x, y, i) == 2 || Ov006_GetPipeLink(x, y, i) == 3)) {
      Ov006_StoreMapCell(NX, NY, Ov006_GetPipeEndCell(NX, NY));
    } else if (CELL_IS_PAIR(CELL_AT(NX, NY), 0x162)) {
      Ov006_StoreMapCell(NX, NY, Ov006_GetPipeEndCell(NX, NY));
    }
  }
}

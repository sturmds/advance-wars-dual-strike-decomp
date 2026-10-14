#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */
void Ov006_SetCellAndFit(int x, int y, int cell);
void Ov006_RefitCellToRight(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* Map cell codes this file handles. The block cells form a 3x3 picture; the
 * _ALT codes sit left of it in the cell set and are handled like the column
 * next to them. A 2x2 block uses the four corner codes. */
#define BLOCK_TOP_LEFT_ALT 36
#define BLOCK_TOP_LEFT 37
#define BLOCK_TOP 38
#define BLOCK_TOP_RIGHT 39
#define BLOCK_LEFT_ALT 68
#define BLOCK_LEFT 69
#define BLOCK_CENTER 70
#define BLOCK_RIGHT 71
#define BLOCK_BOTTOM_LEFT_ALT 100
#define BLOCK_BOTTOM_LEFT 101
#define BLOCK_BOTTOM 102
#define BLOCK_BOTTOM_RIGHT 103
#define CELL_WOOD 135 /* the cell Ov006_FillMapForScreen uses for wood */

/* Called before the cell at x, y is overwritten; state->oldCell holds the
 * cell that was there. If it was part of a 3x3 or 2x2 block of the cells
 * above, rebuilds what is left: a 3x3 block that loses an edge or corner
 * cell shrinks to a 2x2 block of corner cells, and the cells that no longer
 * fit become wood. Last, Ov006_RefitCellToRight refits the tile right of x, y. */
void Ov006_BreakCellBlock(int x, int y) {
  int cell;

  switch (data_ov006_0236d640->oldCell) {
    case BLOCK_CENTER:
      Ov006_SetCellAndFit(x - 1, y - 1, CELL_WOOD);
      Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
      Ov006_SetCellAndFit(x + 1, y - 1, CELL_WOOD);
      Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
      Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
      Ov006_SetCellAndFit(x - 1, y + 1, CELL_WOOD);
      Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
      Ov006_SetCellAndFit(x + 1, y + 1, CELL_WOOD);
      break;
    case BLOCK_BOTTOM_RIGHT:
      cell = CELL_AT(x - 1, y - 1);
      if (cell == BLOCK_CENTER) {
        Ov006_SetCellAndFit(x - 2, y - 2, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x - 1, y - 2, BLOCK_TOP_RIGHT);
        Ov006_SetCellAndFit(x, y - 2, CELL_WOOD);
        Ov006_SetCellAndFit(x - 2, y - 1, BLOCK_BOTTOM_LEFT);
        Ov006_StoreMapCell(x - 1, y - 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x - 2, y, CELL_WOOD);
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
      } else if (cell == BLOCK_LEFT_ALT || cell == BLOCK_LEFT) {
        Ov006_SetCellAndFit(x - 1, y - 2, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x, y - 2, BLOCK_TOP_RIGHT);
        Ov006_SetCellAndFit(x - 1, y - 1, BLOCK_BOTTOM_LEFT);
        Ov006_StoreMapCell(x, y - 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
      } else if (cell == BLOCK_TOP_LEFT_ALT || cell == BLOCK_TOP_LEFT) {
        Ov006_SetCellAndFit(x - 1, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
      }
      break;
    case BLOCK_BOTTOM_LEFT_ALT:
    case BLOCK_BOTTOM_LEFT:
      cell = CELL_AT(x + 1, y - 1);
      if (cell == BLOCK_CENTER) {
        Ov006_SetCellAndFit(x, y - 2, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y - 2, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x + 2, y - 2, BLOCK_TOP_RIGHT);
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y - 1, BLOCK_BOTTOM_LEFT);
        Ov006_StoreMapCell(x + 2, y - 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x + 2, y, CELL_WOOD);
      } else if (cell == BLOCK_RIGHT) {
        Ov006_SetCellAndFit(x, y - 2, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x + 1, y - 2, BLOCK_TOP_RIGHT);
        Ov006_SetCellAndFit(x, y - 1, BLOCK_BOTTOM_LEFT);
        Ov006_StoreMapCell(x + 1, y - 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
      } else if (cell == BLOCK_TOP_RIGHT) {
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
      }
      break;
    case BLOCK_TOP_RIGHT:
      cell = CELL_AT(x - 1, y + 1);
      if (cell == BLOCK_CENTER) {
        Ov006_StoreMapCell(x - 1, y, BLOCK_TOP_RIGHT);
        Ov006_StoreMapCell(x - 1, y + 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x - 2, y + 1, BLOCK_BOTTOM_LEFT);
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 2, CELL_WOOD);
        Ov006_SetCellAndFit(x - 2, y + 2, CELL_WOOD);
        Ov006_SetCellAndFit(x - 1, y + 2, CELL_WOOD);
      } else if (cell == BLOCK_BOTTOM_LEFT_ALT || cell == BLOCK_BOTTOM_LEFT) {
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x - 1, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
      }
      break;
    case BLOCK_TOP_LEFT_ALT:
    case BLOCK_TOP_LEFT:
      cell = CELL_AT(x + 1, y + 1);
      if (cell == BLOCK_BOTTOM_RIGHT) {
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y + 1, CELL_WOOD);
      } else if (cell == BLOCK_RIGHT) {
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 1, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x + 1, y + 1, BLOCK_TOP_RIGHT);
      } else if (cell == BLOCK_CENTER) {
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 2, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y, BLOCK_TOP_LEFT);
        Ov006_SetCellAndFit(x + 1, y + 1, BLOCK_BOTTOM_LEFT);
        Ov006_SetCellAndFit(x + 1, y + 2, CELL_WOOD);
        Ov006_StoreMapCell(x + 2, y + 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x + 2, y + 2, CELL_WOOD);
      }
      break;
    case BLOCK_LEFT_ALT:
    case BLOCK_LEFT:
      if (CELL_AT(x + 1, y) == BLOCK_CENTER) {
        Ov006_SetCellAndFit(x + 1, y - 1, BLOCK_TOP_LEFT);
        Ov006_SetCellAndFit(x + 1, y, BLOCK_BOTTOM_LEFT);
        Ov006_StoreMapCell(x + 2, y, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 2, y + 1, CELL_WOOD);
      }
      break;
    case BLOCK_RIGHT:
      if (CELL_AT(x - 1, y) == BLOCK_CENTER) {
        Ov006_StoreMapCell(x - 1, y - 1, BLOCK_TOP_RIGHT);
        Ov006_StoreMapCell(x - 1, y, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x - 2, y, BLOCK_BOTTOM_LEFT);
        Ov006_SetCellAndFit(x, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x - 1, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x - 2, y + 1, CELL_WOOD);
      }
      break;
    case BLOCK_BOTTOM:
      if (CELL_AT(x, y - 1) == BLOCK_CENTER) {
        Ov006_StoreMapCell(x, y - 2, BLOCK_TOP_RIGHT);
        Ov006_StoreMapCell(x, y - 1, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x - 1, y - 1, BLOCK_BOTTOM_LEFT);
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y - 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y - 2, CELL_WOOD);
      }
      break;
    case BLOCK_TOP:
      if (CELL_AT(x, y + 1) == BLOCK_CENTER) {
        Ov006_SetCellAndFit(x - 1, y + 1, BLOCK_TOP_LEFT);
        Ov006_StoreMapCell(x, y + 1, BLOCK_TOP_RIGHT);
        Ov006_StoreMapCell(x, y + 2, BLOCK_BOTTOM_RIGHT);
        Ov006_SetCellAndFit(x - 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y + 1, CELL_WOOD);
        Ov006_SetCellAndFit(x + 1, y + 2, CELL_WOOD);
      }
      break;
  }
  Ov006_RefitCellToRight(x, y);
}

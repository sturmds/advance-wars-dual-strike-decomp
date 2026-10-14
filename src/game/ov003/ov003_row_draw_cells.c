#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_02364a84.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numerator, s32 denominator);
void Sprite_Queue(int a0, int x, int y, void* a3, int attr, int tile);
int Ov003_RowKindGroup(int value);

extern Unk_027c0280* data_027c0280;
extern Ov003Unit data_ov003_02369538[];

#define STATE data_ov003_02364928
#define ROW (STATE->rows[row - 1])

/* Draws the 20 cells of a row (1 to 5) into the layer 0 tile map, as two
 * columns of 10 cells of 2x2 tiles from tile column ROW.unk_3b_4 * 4 + 1.
 * An empty cell clears its tiles. A cell holding a unit (state 2) also gets
 * a digit sprite with the unit's hp in tens, rounded up, while that is 1 to
 * 9; at 0 it draws like state 3. With phase at 1, 3 or 4 the cells of
 * rows other than the current slot's draw as state 1. */
void Ov003Row_DrawCells(int row) {
  Ov003Cell* cell;
  u16* map;
  int tile;
  int i;
  int hpTenths;
  int column;

  cell = ROW.cells;
  column = ROW.unk_3b_4 * 4;
  for (i = 0; i < OV003_ROW_CELL_COUNT; i++, cell++) {
    if (i < OV003_COLUMN_CELL_COUNT) {
      map = data_027c0280->layer0Map + (column + 1) + (i * 2 + 1) * 32;
    } else {
      map = data_027c0280->layer0Map + (column + 4) +
            ((i - OV003_COLUMN_CELL_COUNT) * 2 + 1) * 32;
    }
    if (cell->state == 0) {
      map[0] = 0;
      map[1] = 0;
      map[32] = 0;
      map[33] = 0;
      continue;
    }
    /* first of the four tiles for this kind of cell, then + 4 per animation frame */
    switch (cell->kind) {
      case 1:
      case 2:
        if (Ov003_RowKindGroup(ROW.co) == 5) {
          tile = 0x270;
        } else {
          tile = 0x200;
        }
        break;
      case 3:
        tile = 0x21c;
        break;
      case 4:
        tile = 0x238;
        break;
      case 5:
        tile = 0x254;
        break;
      default:
        continue;
    }
    switch (cell->state) {
      case 1:
      plain:
        tile += data_ov003_02364a84->unk_0c_0 * 4;
        break;
      case 3:
        tile += 24;
        break;
      case 2:
        if ((STATE->phase == 1 || STATE->phase == 3 || STATE->phase == 4) &&
            row != STATE->slots[STATE->currentSlot].row) {
          goto plain;
        }
        /* needed to match: the unit index is read as signed */
        if (data_ov003_02369538[(s8)cell->unit].hp != 0) {
          hpTenths =
              HwDiv32(data_ov003_02369538[(s8)cell->unit].hp - 1, 10) + 1;
        } else {
          hpTenths = 0;
        }
        if (hpTenths == 0) {
          tile += 24;
          break;
        }
        /* the digit tiles start at 190, two per digit */
        if (hpTenths <= 9) {
          if (i < OV003_COLUMN_CELL_COUNT) {
            Sprite_Queue(1, ((column + 2) * 8 + 4) & 0x1ff,
                         ((i * 2 + 2) * 8 + 1) & 0xff, &data_027c0028,
                         (hpTenths * 2 + 188) | 0x1000, 0);
          } else {
            Sprite_Queue(1, ((column + 5) * 8 + 4) & 0x1ff,
                         (((i - 10) * 2 + 2) * 8 + 1) & 0xff, &data_027c0028,
                         (hpTenths * 2 + 188) | 0x1000, 0);
          }
        }
        tile += (data_ov003_02364a84->unk_0c_4 + 3) * 4;
        break;
      default:
        continue;
    }
    map[0] = tile | ((ROW.color + 2) << 12);
    map[1] = (tile + 1) | ((ROW.color + 2) << 12);
    map[32] = (tile + 2) | ((ROW.color + 2) << 12);
    map[33] = (tile + 3) | ((ROW.color + 2) << 12);
  }
}

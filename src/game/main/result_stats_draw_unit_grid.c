#include "types.h"
#include "game/task.h"
#include "game/oam_cells.h"

/* y is u8 here: the original narrows it at the call, and an explicit
 * `y & 0xff` gets hoisted out of the column loop instead. */
void Sprite_QueueAt(int list, int x, u8 y, void* data, int attr, int tile);
int Player_GetUnitIconTile(int player, int unitType);
int Player_GetUnitsBuilt(int player, int unitType);
int Player_GetUnitsLost(int player, int unitType);
BOOL Sprite_IsOnScreenX(int x, int width);
void ResultStats_DrawCount(int x, int y, int value);

/* data_0215a51c: unit types, UNIT_GRID_ROWS rows of UNIT_GRID_COLUMNS; 0
 * leaves a cell empty. */
#define UNIT_GRID_ROWS 7
#define UNIT_GRID_COLUMNS 4
extern const u8 data_0215a51c[];

/* Draws the unit grid of a stats page for player task->unk_66, moved right
 * by `offset` pixels: each unit's icon and a count. task->unk_68 picks the
 * count (0: Player_GetUnitsBuilt, 1: Player_GetUnitsLost); units with a count of 0 are
 * left out. */
void ResultStats_DrawUnitGrid(int offset, Task* task) {
  int row;
  int col;
  int x;
  u8 type;

  for (row = 0; row < UNIT_GRID_ROWS; row++) {
    for (col = 0; col < UNIT_GRID_COLUMNS; col++) {
      type = data_0215a51c[row * UNIT_GRID_COLUMNS + col];
      if (type == 0) {
        continue;
      }
      if (task->unk_68 == 0 && Player_GetUnitsBuilt(task->unk_66, type) == 0) {
        continue;
      }
      if (task->unk_68 == 1 && Player_GetUnitsLost(task->unk_66, type) == 0) {
        continue;
      }
      x = col * 54 + 25 + offset;
      if (!Sprite_IsOnScreenX(x, 40)) {
        continue;
      }
      Sprite_QueueAt(0, x & 0x1ff, 48 + row * 16, &data_027c0040,
                     (task->unk_66 + 1) << 12,
                     Player_GetUnitIconTile(task->unk_66, type) + 134);
      if (task->unk_68 == 0) {
        ResultStats_DrawCount(x + 16, 48 + row * 16,
                              Player_GetUnitsBuilt(task->unk_66, type));
      } else {
        ResultStats_DrawCount(x + 16, 48 + row * 16,
                              Player_GetUnitsLost(task->unk_66, type));
      }
    }
  }
}

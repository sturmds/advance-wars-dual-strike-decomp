#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_marker.h"

int func_ov003_02362098(int row);
void Ov003_StartSlotPlacing(int unitIndex);
void Ov003_DefeatTeam(int row);
int Ov003_SpawnItem(int x, int y);
int Ov003_IsRowDone(void);

/* Needed to match: the original keeps Ov003Cell.unit signed (-1 is stored
 * with mvn and the compare uses ldrsb). */
#define CELL_UNIT(cell) (*(s8*)&(cell)->unit)

/* Ends unit's presence on the map. It clears the unit from the data_ov003_02369bb8
 * entries that point to it and, when Unk_02160ac0.unk_7c is set and the unit's
 * row is the current one, starts the first free countdown in
 * Ov003State.unk_1c0 at 120. The cell of its row that links to it becomes
 * state 3 with unit -1. A unit of another row then only goes to
 * Ov003_StartSlotPlacing. For a unit of the current row, it calls
 * Ov003_DefeatTeam when Ov003_IsRowDone says so, counts the removal in
 * Ov003State.unk_1d9 and, from the fourth on, resets the count when
 * Ov003_SpawnItem accepts the unit's position. */
void Ov003Unit_Remove(Ov003Unit* unit) {
  Ov003Row* row = &data_ov003_02364928->rows[unit->row - 1];
  int index;
  Ov003Marker* entry;
  Ov003Cell* cell;
  int i;
  int j;

  index = unit - data_ov003_02369538;
  unit->unk_17_7 = 0;
  entry = data_ov003_02369bb8;
  for (i = 0; i < OV003_MARKER_COUNT; i++, entry++) {
    if (entry->busy && entry->target == unit) {
      entry->target = NULL;
    }
  }
  if (data_02160ac0->unk_7c != 0 && unit->row == data_ov003_02364928->row) {
    for (j = 0; j < (int)ARRAY_COUNT(data_ov003_02364928->unk_1c0); j++) {
      if (data_ov003_02364928->unk_1c0[j] == 0) {
        data_ov003_02364928->unk_1c0[j] = 120;
        break;
      }
    }
  }
  cell = row->cells;
  for (i = 0; i < OV003_ROW_CELL_COUNT; i++, cell++) {
    if (cell->state == 2 && CELL_UNIT(cell) == index) {
      cell->state = 3;
      CELL_UNIT(cell) = -1;
      if (func_ov003_02362098(unit->row) == 1) {
        Ov003_StartSlotPlacing(index);
        return;
      }
      if (Ov003_IsRowDone() == 1) {
        Ov003_DefeatTeam(data_ov003_02364928->row);
      }
      data_ov003_02364928->unk_1d9++;
      if (data_ov003_02364928->unk_1d9 >= 4) {
        if (Ov003_SpawnItem(unit->x, unit->y) == 1) {
          data_ov003_02364928->unk_1d9 = 0;
        }
      }
      return;
    }
  }
}

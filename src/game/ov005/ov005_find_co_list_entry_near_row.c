#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

int Ov005_FindCoListEntryAt(int column, int row);
int Ov005_IsCoListEntryActive(Unk_ov005_0236bba0* work, int entry);
u8 Ov005_GetCoListEntryRow(int entry);
u8 Ov005_GetCoListEntryColumn(int entry);
void Ov005_GetCursorRanges(int column, int row, int* minColumn, int* maxColumn,
                           int* minRow, int* maxRow);

/* Looks for an entry with a button in the column of *entry, trying the rows
 * around the current one in the order row-1, row+1, row-2, row+2, ... within
 * the column's row range. Sets *entry to the entry found and returns 1, or
 * puts *entry back and returns 0 after trying each other row. */
int Ov005_FindCoListEntryNearRow(Unk_ov005_0236bba0* work, int* entry) {
  int candidate;
  int tries;
  int start;
  int minRow;
  int column;
  int row;
  int step;
  int maxRow;
  int minColumn;
  int maxColumn;
  start = *entry;
  column = Ov005_GetCoListEntryColumn(start);
  row = Ov005_GetCoListEntryRow(start);

  Ov005_GetCursorRanges(column, row, &minColumn, &maxColumn, &minRow, &maxRow);
  step = 2;
  /* each of the other rows once */
  for (tries = 0; tries < OV005_CO_LIST_ROWS - 1; tries++) {
    int lowest = minRow;
    int highest = maxRow;

    do {
      if (step & 1) {
        candidate = row + (step >> 1);
      } else {
        candidate = row - (step >> 1);
      }
      step++;
    } while (candidate < lowest || candidate > highest);
    *entry = Ov005_FindCoListEntryAt(column, candidate);
    if (Ov005_IsCoListEntryActive(work, *entry) != 0) {
      return 1;
    }
  }
  *entry = start;
  return 0;
}

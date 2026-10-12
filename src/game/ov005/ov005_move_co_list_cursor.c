#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba0.h"

int Ov005_FindCoListEntryAt(int column, int row);
int Ov005_FindCoListEntryNearRow(Unk_ov005_0236bba0* work, int* entry);
int Ov005_MoveCoListCursorVertical(Unk_ov005_0236bba0* work, int* entry,
                                   int direction);
void Ov005_GetCursorRanges(int column, int row, int* minColumn, int* maxColumn,
                           int* minRow, int* maxRow);
int Ov005_IsCoListEntryActive(Unk_ov005_0236bba0* work, int entry);
u8 Ov005_GetCoListEntryRow(int entry);
u8 Ov005_GetCoListEntryColumn(int entry);

extern Unk_ov005_0236bba0* data_ov005_0236bba0;

/* Moves the CO list cursor with the d-pad. Left and right move 11 entries
 * at a time (one column), wrapping at the ends, starting from the row in
 * work->cursorRow; if that entry has no button, a nearby row of the new column
 * is tried, and then the next column. Up and down move within the column
 * and remember the new row. The button of the new entry is selected. */
void Ov005_MoveCoListCursor(void) {
  Unk_ov005_0236bba0* work = data_ov005_0236bba0;
  int entry = work->selectedEntry;
  int vertical;
  int tries;
  int horizontal;
  int found;
  int column;
  int row;
  int minRow;
  int maxRow;
  int minColumn;
  int maxColumn;
  int current; /* needed to match: entry read once into a register */

  vertical = 0;
  if (gKeypad->repeat & PAD_KEY_UP) {
    vertical = 1;
  } else if (gKeypad->repeat & PAD_KEY_DOWN) {
    vertical = 2;
  }
  horizontal = 0;
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    horizontal = 1;
  } else if (gKeypad->repeat & PAD_KEY_RIGHT) {
    horizontal = 2;
  }

  found = 0;
  if (horizontal != 0) {
    entry = Ov005_FindCoListEntryAt(Ov005_GetCoListEntryColumn(entry),
                                    work->cursorRow);
    for (tries = 0; tries < 2; tries++) {
      current = entry;
      column = Ov005_GetCoListEntryColumn(current);
      row = Ov005_GetCoListEntryRow(current);
      Ov005_GetCursorRanges(column, row, &minColumn, &maxColumn, &minRow,
                            &maxRow);
      if (horizontal == 1) {
        if (column > minColumn) {
          entry -= 11;
        } else {
          entry += maxColumn * 11;
        }
      } else {
        if (column < maxColumn) {
          entry += 11;
        } else {
          entry -= maxColumn * 11;
        }
      }
      if (Ov005_IsCoListEntryActive(work, entry) != 0) {
        found = 1;
        break;
      }
      /* the ranges are fetched again but not used */
      current = entry;
      column = Ov005_GetCoListEntryColumn(current);
      row = Ov005_GetCoListEntryRow(current);
      Ov005_GetCursorRanges(column, row, &minColumn, &maxColumn, &minRow,
                            &maxRow);
      found = Ov005_FindCoListEntryNearRow(work, &entry);
      if (found != 0) {
        break;
      }
    }
  } else if (vertical != 0) {
    found = Ov005_MoveCoListCursorVertical(work, &entry, vertical);
    work->cursorRow = Ov005_GetCoListEntryRow(entry);
  }

  if (found == 0) {
    entry = work->selectedEntry;
  }
  if (entry != work->selectedEntry) {
    MenuButton_Select(work->buttonIds[entry]);
  }
  work->selectedEntry = entry;
}

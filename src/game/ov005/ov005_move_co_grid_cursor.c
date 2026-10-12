#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba8.h"

int Ov005_FindCoGridCell(Unk_ov005_0236bba8* work, int col, int row);
int func_ov005_023634b0(Unk_ov005_0236bba8* work, int cell);

extern Unk_ov005_0236bba8* data_ov005_0236bba8;

/* Moves the cursor over the CO grid (4 columns, 7 rows) with the d-pad,
 * wrapping at the edges. Left and right go back to the row kept in
 * unk_38. When the new cell cannot be selected, it walks down (or up)
 * the column, then on to the next column, until it finds one that can.
 * A cell's buttonId holds its MenuButton id. */
void Ov005_MoveCoGridCursor(void) {
  u16 keys;
  Unk_ov005_0236bba8* work;
  int cursor;
  int vertical;
  s16 col;
  int horizontal;
  int i;
  int j;
  s16 row;

  work = data_ov005_0236bba8;
  keys = gKeypad->repeat;

  vertical = 0;
  if (keys & PAD_KEY_UP) {
    vertical = 1;
  } else if (keys & PAD_KEY_DOWN) {
    vertical = 2;
  }
  horizontal = 0;
  if (keys & PAD_KEY_LEFT) {
    horizontal = 1;
  } else if (keys & PAD_KEY_RIGHT) {
    horizontal = 2;
  }

  cursor = work->cursor;
  row = work->grid[cursor].row;
  if (vertical != 0) {
    if (vertical == 1) {
      if (row > 0) {
        cursor -= 4;
      } else {
        cursor += 24;
      }
    } else if (vertical == 2) {
      if (row < 6) {
        cursor += 4;
      } else {
        cursor -= 24;
      }
    }
  }
  if (horizontal != 0) {
    col = work->grid[cursor].column;
    if (work->cursorRow != -1) {
      cursor = Ov005_FindCoGridCell(work, col, work->cursorRow);
    }
    if (horizontal == 1) {
      if (col > 0) {
        cursor--;
      } else {
        cursor += 3;
      }
    } else if (horizontal == 2) {
      if (col < 3) {
        cursor++;
      } else {
        cursor -= 3;
      }
    }
  }

  if (func_ov005_023634b0(work, cursor)) {
    if (cursor != work->cursor) {
      MenuButton_Select(work->grid[cursor].buttonId);
    }
    work->cursor = cursor;
    if (vertical != 0) {
      work->cursorRow = work->grid[cursor].row;
    }
    return;
  }

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 7; j++) {
      col = work->grid[cursor].column;
      row = work->grid[cursor].row;
      if (vertical == 2 || horizontal == 2) {
        if (row < 6) {
          cursor += 4;
        } else {
          cursor -= 24;
        }
      } else {
        if (row > 0) {
          cursor -= 4;
        } else {
          cursor += 24;
        }
      }
      /* the original repeats the block above here */
      if (func_ov005_023634b0(work, cursor)) {
        if (cursor != work->cursor) {
          MenuButton_Select(work->grid[cursor].buttonId);
        }
        work->cursor = cursor;
        if (vertical != 0) {
          work->cursorRow = work->grid[cursor].row;
        }
        return;
      }
    }
    if (horizontal == 2) {
      if (col < 3) {
        cursor++;
      } else {
        cursor -= 3;
      }
    } else {
      if (col > 0) {
        cursor--;
      } else {
        cursor += 3;
      }
    }
  }
}

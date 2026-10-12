#include "types.h"
#include "game/force_rank_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void ForceRankScreen_ClearChosen(void);
void ForceRankScreen_NextSlot(void);
void ForceRankScreen_PrevSlot(void);
void ForceRankScreen_Close(void);

#define CURSOR_COLUMN(cursor) ((cursor) >> 3)
#define CURSOR_ROW(cursor) ((cursor) & 7)

/* onKeys of the Force Rank selection buttons. Up and down wrap within the
 * column; left and right skip empty columns (left wraps from column 1 to
 * the last one) and keep the row when the new column has it. L and R call
 * ForceRankScreen_PrevSlot and ForceRankScreen_NextSlot, X calls ForceRankScreen_ClearChosen. */
void ForceRankScreen_HandleKeys(MenuButton* button) {
  int cursor;
  int column;
  u16 keys;

  cursor = button->id;
  if (gKeypad->repeat & PAD_KEY_UP) {
    if (CURSOR_ROW(cursor) != 0) {
      cursor--;
    } else {
      cursor = (cursor & ~7) |
               (gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1);
    }
    gForceRankScreen->row = CURSOR_ROW(cursor);
    if (CURSOR_COLUMN(cursor) == FORCE_RANK_COLUMN_NONE) {
      gForceRankScreen->row = 5;
    }
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (CURSOR_ROW(cursor) <
        gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1) {
      cursor++;
    } else {
      cursor &= ~7;
    }
    gForceRankScreen->row = CURSOR_ROW(cursor);
    if (CURSOR_COLUMN(cursor) == FORCE_RANK_COLUMN_NONE) {
      gForceRankScreen->row = 5;
    }
  }
  keys = gKeypad->repeat;
  if (keys & PAD_KEY_LEFT) {
    column = CURSOR_COLUMN(cursor);
    do {
      if (column > 1) {
        column--;
      } else {
        column = FORCE_RANK_COLUMN_NONE;
      }
    } while (gForceRankScreen->columns[column].count == 0);
    cursor = (column << 3) | gForceRankScreen->row;
    if (CURSOR_ROW(cursor) >
        gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1) {
      cursor = (column << 3) |
               (gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1);
    }
  }
  if (keys & PAD_KEY_RIGHT) {
    column = CURSOR_COLUMN(cursor);
    do {
      if (column < FORCE_RANK_COLUMN_NONE) {
        column++;
      } else {
        column = 0;
      }
    } while (gForceRankScreen->columns[column].count == 0);
    cursor = (column << 3) | gForceRankScreen->row;
    if (CURSOR_ROW(cursor) >
        gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1) {
      cursor = (column << 3) |
               (gForceRankScreen->columns[CURSOR_COLUMN(cursor)].count - 1);
    }
  }
  if (cursor != button->id) {
    button = MenuButton_Select(cursor);
  }
  if (gKeypad->pressed & PAD_BUTTON_L) {
    ForceRankScreen_PrevSlot();
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    ForceRankScreen_NextSlot();
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  }
  if (gKeypad->pressed & PAD_BUTTON_X) {
    ForceRankScreen_ClearChosen();
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    ForceRankScreen_Close();
  }
}

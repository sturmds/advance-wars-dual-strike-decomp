#include "types.h"
#include "game/force_rank_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"

MenuButton *func_020262b0(int id);
void func_020250a8(MenuButton *button);
void func_02040568(void);
void func_0204062c(void);
void func_0204069c(void);
void ForceRankScreen_Close(void);

#define CURSOR_COLUMN(cursor) ((cursor) >> 3)
#define CURSOR_ROW(cursor)    ((cursor) & 7)

/* onKeys of the Force Rank selection buttons. Up and down wrap within the
 * column; left and right skip empty columns (left wraps from column 1 to
 * the last one) and keep the row when the new column has it. L and R call
 * func_0204069c and func_0204062c, X calls func_02040568. */
void ForceRankScreen_HandleKeys(MenuButton *button)
{
    int cursor;
    int column;
    u16 keys;

    cursor = button->id;
    if (data_0216e4e0->repeat & PAD_KEY_UP) {
        if (CURSOR_ROW(cursor) != 0) {
            cursor--;
        } else {
            cursor = (cursor & ~7) | (data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1);
        }
        data_0229c738->row = CURSOR_ROW(cursor);
        if (CURSOR_COLUMN(cursor) == FORCE_RANK_LAST_COLUMN) {
            data_0229c738->row = 5;
        }
    }
    if (data_0216e4e0->repeat & PAD_KEY_DOWN) {
        if (CURSOR_ROW(cursor) < data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1) {
            cursor++;
        } else {
            cursor &= ~7;
        }
        data_0229c738->row = CURSOR_ROW(cursor);
        if (CURSOR_COLUMN(cursor) == FORCE_RANK_LAST_COLUMN) {
            data_0229c738->row = 5;
        }
    }
    keys = data_0216e4e0->repeat;
    if (keys & PAD_KEY_LEFT) {
        column = CURSOR_COLUMN(cursor);
        do {
            if (column > 1) {
                column--;
            } else {
                column = FORCE_RANK_LAST_COLUMN;
            }
        } while (data_0229c738->columns[column].count == 0);
        cursor = (column << 3) | data_0229c738->row;
        if (CURSOR_ROW(cursor) > data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1) {
            cursor = (column << 3) | (data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1);
        }
    }
    if (keys & PAD_KEY_RIGHT) {
        column = CURSOR_COLUMN(cursor);
        do {
            if (column < FORCE_RANK_LAST_COLUMN) {
                column++;
            } else {
                column = 0;
            }
        } while (data_0229c738->columns[column].count == 0);
        cursor = (column << 3) | data_0229c738->row;
        if (CURSOR_ROW(cursor) > data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1) {
            cursor = (column << 3) | (data_0229c738->columns[CURSOR_COLUMN(cursor)].count - 1);
        }
    }
    if (cursor != button->id) {
        button = func_020262b0(cursor);
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_L) {
        func_0204069c();
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_R) {
        func_0204062c();
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_A) {
        func_020250a8(button);
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_X) {
        func_02040568();
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_B) {
        ForceRankScreen_Close();
    }
}

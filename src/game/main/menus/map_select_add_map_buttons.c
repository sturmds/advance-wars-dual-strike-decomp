#include "types.h"
#include "game/map_select.h"

void MenuButton_Add(int x, int y, void *def, int id);
void *MenuButton_Select(int id);
void MenuButtons_RemoveSelectable(void);
int func_0207bd6c(void);
int func_0207bd80(void);

extern u8 data_0215c8a0[];

#define FIRST_ROW() (STATE->gameMode == 1 ? 9 : (STATE->twoMaps == 0 ? 5 : 7))

/* Adds one button per map of the current page, in columns of COLS buttons
 * 2 tiles apart (the next column starts 9 tiles right, except in game mode 1,
 * where there is a single column), then selects the button of the chosen
 * map. Returns at once when func_0207bd80() is 1 and func_0207bd6c() is nonzero
 * (their meaning is unknown). Needed to match: the list index sits in a local. */
void MapSelect_AddMapButtons(void)
{
    int x;
    int y;
    int i;
    int index;

    x = STATE->gameMode == 1 ? 17 : 1;
    y = FIRST_ROW();
    if (func_0207bd80() == 1 && func_0207bd6c() != 0) {
        return;
    }
    MenuButtons_RemoveSelectable();
    for (i = 0; i < ROWS * COLS; i++) {
        index = PAGE() * (ROWS * COLS) + i;
        if (index >= LIST(TAB()).mapCount) {
            break;
        }
        MenuButton_Add(x * 8, y * 8, data_0215c8a0, i);
        y += 2;
        if (y == FIRST_ROW() + COLS * 2) {
            x += STATE->gameMode == 1 ? 0 : 9;
            y = FIRST_ROW();
        }
    }
    MenuButton_Select(TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()]);
}

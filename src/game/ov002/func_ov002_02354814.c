#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void func_ov002_0235d514(int side);

/* Needed to match: the entries of a side as seen from the start of the menu
 * state. The original keeps the offset 0x310 in the load, not in the address. */
typedef struct SideEntries {
    u8 pad_000[0x310];
    BattleEntry entries[5];
} SideEntries;

typedef struct EntryAt {
    u8 pad_000[0x325];
    u8 fillAfter;
    u8 fillShown;
} EntryAt;

#define SIDE_ENTRIES(side) ((SideEntries *)((u8 *)&gMenuState + (side) * 120))

/* Runs one step of a side's HP drain: the cell named by the step shows its
 * final fill if no later step names the same cell, and otherwise at least 1
 * when the final fill is 0. Partial: 95.7%, the store address is built twice
 * in the original (cell * 24 shared, side * 120 not). The s8 and u8 locals
 * (found by permute.py) put fillAfter and isLast in the right registers. */
void func_ov002_02354814(int side, int step)
{
    BattleStepList *list;
    s8 cell;
    EntryAt *entry;
    u8 isLast;
    u8 fillAfter;

    list = (BattleStepList *)((u8 *)&gMenuState + 0x2c8) + side;
    cell = list->steps[step].cell;

    func_ov002_0235d514(side);
    entry = (EntryAt *)((u8 *)&gMenuState + cell * 24 + side * 120);
    fillAfter = entry->fillAfter;
    if (entry->fillShown == fillAfter) {
        return;
    }
    isLast = TRUE;
    for (step++; step < list->count; step++) {
        if (cell == list->steps[step].cell) {
            isLast = FALSE;
            break;
        }
    }
    if (isLast == TRUE) {
        SIDE_ENTRIES(side)->entries[cell].fillShown = fillAfter;
    } else if (fillAfter == 0) {
        entry->fillShown = 1;
    }
}

#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

int func_ov005_0235d4fc(Unk_ov005_0236bba0 *work, int entry);
u8 func_ov005_0235d5bc(int entry);
u8 func_ov005_0235d5cc(int entry);
void Ov005_GetCursorRanges(int column, int row, int *minColumn, int *maxColumn, int *minRow, int *maxRow);

/* Moves *entry one step up (direction 1) or down (direction 2) within its
 * column, wrapping at the ends, and repeats up to 10 times until it lands on
 * an entry with a button. Returns 1 if it found one. Ov005_GetCursorRanges
 * gives the column and row ranges (guessed from its constants). */
int Ov005_MoveCoListCursorVertical(Unk_ov005_0236bba0 *work, int *entry, int direction)
{
    int tries;

    for (tries = 0; tries < 10; tries++) {
        int row = *entry;
        int column;
        int minRow;
        int maxRow;
        int minColumn;
        int maxColumn;

        column = func_ov005_0235d5cc(row);
        row = func_ov005_0235d5bc(row);

        Ov005_GetCursorRanges(column, row, &minColumn, &maxColumn, &minRow, &maxRow);
        if (direction == 1) {
            if (row > minRow) {
                *entry -= 1;
            } else {
                *entry += maxRow;
            }
        } else if (direction == 2) {
            if (row < maxRow) {
                *entry += 1;
            } else {
                *entry -= maxRow;
            }
        }
        if (func_ov005_0235d4fc(work, *entry) != 0) {
            return 1;
        }
    }
    return 0;
}

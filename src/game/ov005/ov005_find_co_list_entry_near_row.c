#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

int Ov005_FindCoListEntryAt(int column, int row);
int func_ov005_0235d4fc(Unk_ov005_0236bba0 *work, int entry);
u8 func_ov005_0235d5bc(int entry);
u8 func_ov005_0235d5cc(int entry);
void Ov005_GetCursorRanges(int column, int row, int *minColumn, int *maxColumn, int *minRow, int *maxRow);

/* Looks for an entry with a button in the column of *entry, trying the rows
 * around the current one in the order row-1, row+1, row-2, row+2, ... within
 * the column's row range. Sets *entry to the entry found and returns 1, or
 * puts *entry back and returns 0 after 10 tries. */
int Ov005_FindCoListEntryNearRow(Unk_ov005_0236bba0 *work, int *entry)
{
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
    column = func_ov005_0235d5cc(start);
    row = func_ov005_0235d5bc(start);

    Ov005_GetCursorRanges(column, row, &minColumn, &maxColumn, &minRow, &maxRow);
    step = 2;
    for (tries = 0; tries < 10; tries++) {
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
        if (func_ov005_0235d4fc(work, *entry) != 0) {
            return 1;
        }
    }
    *entry = start;
    return 0;
}

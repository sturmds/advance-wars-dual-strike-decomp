#include "types.h"

/* The cursor moves on a grid of 3 rows and 11 columns. Sets the index range
 * the cursor can reach along each axis from its position; -1: no range. */
void Ov005_GetCursorRanges(int row, int column, int *rowMin, int *rowMax, int *columnMin, int *columnMax)
{
    *rowMin = -1;
    *rowMax = -1;
    *columnMin = -1;
    *columnMax = -1;
    switch (row) {
    case 0:
    case 1:
    case 2:
        *columnMin = 0;
        *columnMax = 10;
        break;
    }
    switch (column) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        *rowMin = 0;
        *rowMax = 2;
        break;
    }
}

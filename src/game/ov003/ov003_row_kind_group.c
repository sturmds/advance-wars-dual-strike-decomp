#include "types.h"

/* Maps the low nibble of byte 0x3b of a state row to a group: 1 for kinds 0 to 3, 5 for 4 to 7, 0 otherwise. */
int Ov003_RowKindGroup(int rowKind)
{
    if (rowKind <= 3) {
        return 1;
    }
    if (rowKind <= 7) {
        return 5;
    }
    return 0;
}

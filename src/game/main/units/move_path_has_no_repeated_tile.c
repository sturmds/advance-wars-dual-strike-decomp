#include "types.h"
#include "game/move_path.h"

extern MovePath *data_02166e7c;

/* TRUE when no tile of the move path appears twice. */
BOOL MovePath_HasNoRepeatedTile(void)
{
    s8 i;
    s8 j;

    for (i = data_02166e7c->lastTile; i > 0; i--) {
        for (j = i - 1; j >= 0; j--) {
            if (data_02166e7c->x[i] == data_02166e7c->x[j] && data_02166e7c->y[i] == data_02166e7c->y[j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

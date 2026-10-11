#include "types.h"
#include "game/move_path.h"

extern MovePath *data_02166e7c;

/* Direction (MOVE_DIR_*) from tile i of the move path to tile i + 1, or
 * MOVE_DIR_NONE at the last tile or when the tiles are the same. */
int MovePath_DirToNext(int i)
{
    if (i == data_02166e7c->lastTile) {
        return MOVE_DIR_NONE;
    }
    if (data_02166e7c->x[i] < data_02166e7c->x[i + 1]) {
        return MOVE_DIR_RIGHT;
    }
    if (data_02166e7c->x[i] > data_02166e7c->x[i + 1]) {
        return MOVE_DIR_LEFT;
    }
    if (data_02166e7c->y[i] < data_02166e7c->y[i + 1]) {
        return MOVE_DIR_DOWN;
    }
    if (data_02166e7c->y[i] > data_02166e7c->y[i + 1]) {
        return MOVE_DIR_UP;
    }
    return MOVE_DIR_NONE;
}

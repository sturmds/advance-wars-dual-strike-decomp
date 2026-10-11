#ifndef GAME_MOVE_PATH_H
#define GAME_MOVE_PATH_H

#include "types.h"

/* Tiles of the move path the player draws for a unit (data_02166e7c).
 * Tile 0 is where the unit stands. */
typedef struct MovePath {
    u8 pad_00[0x20];
    s8 x[12];
    s8 y[12];
    u8 pad_38[0x45 - 0x38];
    s8 lastTile;                       /* index of the last tile in x and y */
} MovePath;

/* Steps of a unit path (Unk_02080440_48.unk_170, UnitPath_CountSteps). */
enum {
    PATH_STEP_LEFT,
    PATH_STEP_RIGHT,
    PATH_STEP_DOWN,
    PATH_STEP_UP,
    PATH_STEP_END
};

#endif

#ifndef GAME_MOVE_PATH_H
#define GAME_MOVE_PATH_H

#include "types.h"

/* Tiles of the move path the player draws for a unit (gMovePath).
 * Tile 0 is where the unit stands. */
typedef struct MovePath {
  u8 pad_00[0x1e];
  s8 cursorX; /* cursor tile the path was last updated for (MovePath_SetCursor) */
  s8 cursorY;
  s8 x[12];
  s8 y[12];
  s8 movesLeft[12]; /* move points left on arrival at each tile */
  s8 startMoves; /* move points at tile 0, see MovePath_Truncate; name a guess */
  s8 lastTile;   /* index of the last tile in x and y */
} MovePath;

extern MovePath data_022a6c30; /* what gMovePath points to */

/* Directions between move path tiles (MovePath_DirToNext, MovePath_DirToPrev). */
enum {
  MOVE_DIR_NONE,
  MOVE_DIR_RIGHT,
  MOVE_DIR_DOWN,
  MOVE_DIR_LEFT,
  MOVE_DIR_UP
};

/* Arrow cell drawn on a move path tile, indexed [direction to the next
 * tile][direction from the previous tile] (MOVE_DIR_*). */
extern u16 gMovePathArrowCells[][5];

/* Steps of a unit path (Unk_02080440_48.pathSteps, UnitPath_CountSteps). */
enum {
  PATH_STEP_LEFT,
  PATH_STEP_RIGHT,
  PATH_STEP_DOWN,
  PATH_STEP_UP,
  PATH_STEP_END
};

/* Written by UnitPath_StopAtEnemy where a unit of another team blocks the
 * path; the step after it is the end marker. */
#define PATH_STEP_BLOCKED 10

#endif

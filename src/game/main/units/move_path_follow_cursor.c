#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"
#include "game/move_path.h"
#include "game/range_map.h"

void MovePath_AddTile(s8 x, s8 y);
void MovePath_AppendSteps(void);
void MovePath_Truncate(int count);
void MovePath_SetCursor(u16 x, u16 y);
int MovePath_FindTile(s8 x, s8 y);
void MovePath_FillRangeFromEnd(void);
void MovePath_RebuildToTarget(void);
BOOL MovePath_HasNoRepeatedTile(void);
void RangeMap_TracePath(int x, int y, u8* steps);

extern MovePath* gMovePath;
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PATH gMovePath
/* Owner of the acting unit: slot 0 is neutral, so unit slots 0-63
 * belong to player 1. */
#define ACTING_PLAYER(state) \
  ((state)->players[((state)->actingUnitIndex >> 6) + 1])
#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Follows the map cursor (cursorX, cursorY) with the move path when it moves
 * to another tile in the acting unit's range. A tile already on the path cuts
 * the path back to it; a reachable tile next to the path's end is appended;
 * anything else rebuilds the path by search (RangeMap_TracePath). unk_46 gets the
 * move points the path uses. */
void MovePath_FollowCursor(void) {
  Unk_02080440_48* state;
  s8 range;
  s16 x;
  s16 y;
  int co;
  int i;
  s8 lastX;
  int lastY;

  state = GAME;
  y = state->cursorY;
  x = state->cursorX;
  range = state->rangeMap[y][x];
  if (PATH->cursorX == x && PATH->cursorY == y) {
    return;
  }
  MovePath_SetCursor(x, y);
  if (range == -1) {
    return;
  }

  i = MovePath_FindTile(GAME->cursorX, GAME->cursorY);
  if (i != -1) {
    MovePath_Truncate((s8)(i + 1));
    GAME->unk_46 = PATH->startMoves - PATH->movesLeft[PATH->lastTile];
    return;
  }

  if (GAME->coEffectsOn == 0) {
    co = 0;
  } else {
    co = ACTING_PLAYER(GAME).front.id & PLAYER_CO_ID_MASK;
  }
  x = GAME->cursorX;
  i = PATH->lastTile;
  if (PATH->movesLeft[i] >=
      ((const s8(*)[32])gCoTable[co]
           .blocks[ACTING_PLAYER(GAME).front.powerState]
           .movementCost[ACTING_PLAYER(GAME).moveChart])
          [Ov000_gUnitStats[GAME->actingUnit->type].moveType][MAP_TILE_TERRAIN(
              GAME->mapTiles
                  ->tiles[x + GAME->mapTiles->rowStart[GAME->cursorY]])]) {
    lastX = PATH->x[i];
    /* Needed to match: y set here, not with x, gives the original's
     * registers. */
    y = GAME->cursorY;
    /* Needed to match: through lastY, mwcc compares the path tile first
     * (cmp tile, y); the plain field read puts y first. */
    if ((ABS(lastX - x) == 1 && (lastY = PATH->y[i], lastY == y)) ||
        (ABS(PATH->y[i] - y) == 1 && lastX == x)) {
      MovePath_AddTile(x, y);
      GAME->unk_46 = PATH->startMoves - PATH->movesLeft[PATH->lastTile];
      return;
    }
  }
  /* Out of move points: start over from the unit's tile. */
  if (PATH->movesLeft[i] == 0) {
    MovePath_Truncate(1);
  }

  RangeMap_SetBuffer(GAME->mapTiles->rangeScratch);
  MovePath_FillRangeFromEnd();
  if (GAME->rangeMap[GAME->cursorY][GAME->cursorX] == -1) {
    MovePath_RebuildToTarget();
  } else {
    RangeMap_TracePath(GAME->cursorX, GAME->cursorY, GAME->pathSteps);
    MovePath_AppendSteps();
    if (!MovePath_HasNoRepeatedTile()) {
      MovePath_RebuildToTarget();
    }
  }
  RangeMap_SetBuffer(GAME->mapTiles->rangeMapTiles);
}

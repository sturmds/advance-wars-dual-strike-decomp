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

  state = MAP_STATE;
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

  i = MovePath_FindTile(MAP_STATE->cursorX, MAP_STATE->cursorY);
  if (i != -1) {
    MovePath_Truncate((s8)(i + 1));
    MAP_STATE->unk_46 = PATH->startMoves - PATH->movesLeft[PATH->lastTile];
    return;
  }

  if (MAP_STATE->coEffectsOn == 0) {
    co = 0;
  } else {
    co = ACTING_PLAYER(MAP_STATE).front.id & PLAYER_CO_ID_MASK;
  }
  x = MAP_STATE->cursorX;
  i = PATH->lastTile;
  if (PATH->movesLeft[i] >=
      ((const s8(*)[32])gCoTable[co]
           .blocks[ACTING_PLAYER(MAP_STATE).front.powerState]
           .movementCost[ACTING_PLAYER(MAP_STATE).moveChart])
          [Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveType]
          [MAP_TILE_TERRAIN(
              MAP_STATE->mapTiles->tiles
                  [x + MAP_STATE->mapTiles->rowStart[MAP_STATE->cursorY]])]) {
    lastX = PATH->x[i];
    /* Needed to match: y set here, not with x, gives the original's
     * registers. */
    y = MAP_STATE->cursorY;
    /* Needed to match: through lastY, mwcc compares the path tile first
     * (cmp tile, y); the plain field read puts y first. */
    if ((ABS(lastX - x) == 1 && (lastY = PATH->y[i], lastY == y)) ||
        (ABS(PATH->y[i] - y) == 1 && lastX == x)) {
      MovePath_AddTile(x, y);
      MAP_STATE->unk_46 = PATH->startMoves - PATH->movesLeft[PATH->lastTile];
      return;
    }
  }
  /* Out of move points: start over from the unit's tile. */
  if (PATH->movesLeft[i] == 0) {
    MovePath_Truncate(1);
  }

  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  MovePath_FillRangeFromEnd();
  if (MAP_STATE->rangeMap[MAP_STATE->cursorY][MAP_STATE->cursorX] == -1) {
    MovePath_RebuildToTarget();
  } else {
    RangeMap_TracePath(MAP_STATE->cursorX, MAP_STATE->cursorY,
                       MAP_STATE->pathSteps);
    MovePath_AppendSteps();
    if (!MovePath_HasNoRepeatedTile()) {
      MovePath_RebuildToTarget();
    }
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
}

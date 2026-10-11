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
void func_01ffa02c(int x, int y, u8 *steps);

extern MovePath *gMovePath;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PATH gMovePath
/* Needed to match: the original loads the index before the base here. */
#define RELOAD_STATE(dst)                                                    \
    {                                                                        \
        int index_ = *(volatile int *)&data_027c0284;                        \
        dst = (*(Unk_02080440 *volatile *)&data_027c027c)->unk_48[index_];   \
    }
/* Owner of the acting unit: slot 0 is neutral, so unit slots 0-63
 * belong to player 1. */
#define ACTING_PLAYER(state) ((state)->players[((state)->actingUnitIndex >> 6) + 1])
#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Follows the map cursor (cursorX, cursorY) with the move path when it moves
 * to another tile in the acting unit's range. A tile already on the path cuts
 * the path back to it; a reachable tile next to the path's end is appended;
 * anything else rebuilds the path by search (func_01ffa02c). unk_46 gets the
 * move points the path uses. */
void MovePath_FollowCursor(void)
{
    Unk_02080440_48 *state;
    s8 range;
    s16 x;
    s16 y;
    int co;
    int i;
    s8 lastX;

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

    RELOAD_STATE(state);
    if (state->coEffectsOn == 0) {
        co = 0;
    } else {
        co = ACTING_PLAYER(state).front.id & PLAYER_CO_ID_MASK;
    }
    x = state->cursorX;
    y = state->cursorY;
    i = PATH->lastTile;
    if (PATH->movesLeft[i]
        >= ((const s8 (*)[32])gCoTable[co].blocks[ACTING_PLAYER(state).front.powerState]
                .movementCost[ACTING_PLAYER(state).moveChart])
               [Ov000_gUnitStats[state->actingUnit->type].moveType]
               [MAP_TILE_TERRAIN(state->mapTiles->tiles[x + state->mapTiles->rowStart[y]])]) {
        lastX = PATH->x[i];
        if ((ABS(lastX - x) == 1 && PATH->y[i] == y) || (ABS(PATH->y[i] - y) == 1 && lastX == x)) {
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
    RELOAD_STATE(state);
    if (state->rangeMap[state->cursorY][state->cursorX] == -1) {
        MovePath_RebuildToTarget();
    } else {
        func_01ffa02c(state->cursorX, state->cursorY, state->pathSteps);
        MovePath_AppendSteps();
        if (!MovePath_HasNoRepeatedTile()) {
            MovePath_RebuildToTarget();
        }
    }
    RangeMap_SetBuffer(GAME->mapTiles->rangeMapTiles);
}

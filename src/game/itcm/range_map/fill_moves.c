#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s8 data_027c074c;
extern int data_027c0750;
extern MoveSearchNode *data_027c0754;
extern MoveSearch *data_027c0758;
extern MoveSearchNode *data_027c075c;

void MoveSearch_LoadMoveCosts(int unitType);
/* Queues the tile (dx, dy) away from the node being expanded, coming from
 * direction. */
void RangeMap_QueueStep(int direction, u8 dx, u8 dy);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define FROM_START 1
#define DIR_UP     2
#define DIR_DOWN   3
#define DIR_LEFT   4
#define DIR_RIGHT  5

#define STEP_UP()    RangeMap_QueueStep(DIR_UP, 0, -1)
#define STEP_DOWN()  RangeMap_QueueStep(DIR_DOWN, 0, 1)
#define STEP_LEFT()  RangeMap_QueueStep(DIR_LEFT, -1, 0)
#define STEP_RIGHT() RangeMap_QueueStep(DIR_RIGHT, 1, 0)

/* Fills rangeMap with the cost to reach each tile from (x, y) for a unit
 * type with maxCost movement; -1: out of reach. Oozium gets moveRange 0. */
void RangeMap_FillMoves(int x, int y, int unitType, s16 maxCost, int moveRange)
{
    data_027c0750 = 0;
    data_027c074c = 0;
    data_027c075c = data_027c0758->lists[0];
    data_027c0754 = data_027c0758->lists[1];
    data_027c0758->maxCost = maxCost;
    data_027c0758->moveRange = moveRange;
    /* the team of player unk_1e, in the field MoveSearch_FindTile uses for a unit type */
    data_027c0758->unitType = STATE->players[STATE->unk_1e].team;
    if (unitType == UNIT_OOZIUM) {
        data_027c0758->moveRange = 0;
    }
    RangeMap_Fill(-1);
    MoveSearch_LoadMoveCosts(unitType);
    data_027c075c->x = x;
    data_027c075c->y = y;
    data_027c075c->from = FROM_START;
    data_027c075c->cost = 0;
    STATE->rangeMap[y][x] = 0;
    data_027c075c++;
    data_027c075c->from = 0;
    for (;;) {
        /* swap the lists: expand what was found, collect into the other */
        if (data_027c074c == 0) {
            data_027c074c = 1;
            data_027c075c = data_027c0758->lists[1];
            data_027c0754 = data_027c0758->lists[0];
            data_027c0750 = 0;
        } else {
            data_027c074c = 0;
            data_027c075c = data_027c0758->lists[0];
            data_027c0754 = data_027c0758->lists[1];
            data_027c0750 = 0;
        }
        if (data_027c0754->from == 0) {
            return;
        }
        while (data_027c0754->from != 0) {
            switch (data_027c0754->from) {
            case FROM_START:
                STEP_UP();
                STEP_DOWN();
                STEP_LEFT();
                STEP_RIGHT();
                break;
            case DIR_UP:
                STEP_UP();
                STEP_LEFT();
                STEP_RIGHT();
                break;
            case DIR_DOWN:
                STEP_DOWN();
                STEP_LEFT();
                STEP_RIGHT();
                break;
            case DIR_LEFT:
                STEP_UP();
                STEP_DOWN();
                STEP_LEFT();
                break;
            case DIR_RIGHT:
                STEP_UP();
                STEP_DOWN();
                STEP_RIGHT();
                break;
            }
            data_027c075c->from = 0;
            data_027c0754++;
        }
    }
}

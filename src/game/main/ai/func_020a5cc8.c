#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s8 data_027c074c;
extern int data_027c0750;
extern MoveSearchNode *data_027c0754;
extern MoveSearch *data_027c0758;
extern MoveSearchNode *data_027c075c;
int Player_GetUnitMoveRange(int player, int unitType);
void MoveSearch_LoadMoveCosts(int unitType);
BOOL MoveSearch_TryStepInCost(int direction);
#define STATE (data_027c027c->unk_48[data_027c0284])
#define FROM_START 1
#define DIR_UP     2
#define DIR_DOWN   3
#define DIR_LEFT   4
#define DIR_RIGHT  5

/* MoveSearch_FindTile with MoveSearch_TryStepInCost as the step: same breadth-first
 * search, a different step test. */
BOOL func_020a5cc8(int x, int y, int unitType, int maxCost)
{
    data_027c0750 = 0;
    data_027c074c = 0;
    data_027c075c = data_027c0758->lists[0];
    data_027c0754 = data_027c0758->lists[1];
    data_027c0758->maxCost = maxCost;
    data_027c0758->moveRange = Player_GetUnitMoveRange(STATE->currentPlayer, unitType);
    data_027c0758->unitType = unitType;
    MoveSearch_LoadMoveCosts(unitType);
    data_027c075c->x = x;
    data_027c075c->y = y;
    data_027c075c->from = FROM_START;
    data_027c075c->cost = 0;
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
            break;
        }
        while (data_027c0754->from != 0) {
            switch (data_027c0754->from) {
            case FROM_START:
                if (MoveSearch_TryStepInCost(DIR_UP) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE) goto found;
                break;
            case DIR_UP:
                if (MoveSearch_TryStepInCost(DIR_UP) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE) goto found;
                break;
            case DIR_DOWN:
                if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE) goto found;
                break;
            case DIR_LEFT:
                if (MoveSearch_TryStepInCost(DIR_UP) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE) goto found;
                break;
            case DIR_RIGHT:
                if (MoveSearch_TryStepInCost(DIR_UP) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE) goto found;
                if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE) goto found;
                break;
            }
            data_027c075c->from = 0;
            data_027c0754++;
        }
    }
    data_027c0758->x = STATE->unk_1c4->mapWidth;
    data_027c0758->y = STATE->unk_1c4->mapHeight;
    return FALSE;

found:
    data_027c0758->x = STATE->unk_1c4->mapWidth;
    data_027c0758->y = STATE->unk_1c4->mapHeight;
    return TRUE;
}

#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int GetUnitMovement(int player, int unitType);
void AiQueue_SortAscending(int count);
void func_0208fa5c(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by each unit's movement; smallest first unless
 * descending. */
void AiQueue_SortByMovement(BOOL descending)
{
    int i;
    int type;

    /* needed to match: a plain for condition rotates the loop */
    for (i = 0;; i++) {
        if (gAiQueue[i] == 0) {
            break;
        }
        type = STATE->units[gAiQueue[i]].type;
        if (type == UNIT_BOMBER) {
            /* a Bomber counts as one more than a Fighter */
            data_0229e234[i] = GetUnitMovement(STATE->currentPlayer, UNIT_FIGHTER) + 1;
        } else {
            data_0229e234[i] = GetUnitMovement(STATE->currentPlayer, type);
        }
    }
    if (!descending) {
        AiQueue_SortAscending(i);
    } else {
        func_0208fa5c(i);
    }
}

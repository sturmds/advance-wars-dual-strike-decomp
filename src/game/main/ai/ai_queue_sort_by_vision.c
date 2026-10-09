#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Vision of a unit type for a player, after CO bonuses and rain. */
int func_020e0214(int player, int unitType);
void AiQueue_SortAscending(int count);
void AiQueue_SortDescending(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by each unit's vision; smallest first unless
 * descending. */
void AiQueue_SortByVision(BOOL descending)
{
    int i;

    /* needed to match: a plain for condition rotates the loop */
    for (i = 0;; i++) {
        if (gAiQueue[i] == 0) {
            break;
        }
        data_0229e234[i] = func_020e0214(STATE->currentPlayer, STATE->units[gAiQueue[i]].type);
    }
    if (!descending) {
        AiQueue_SortAscending(i);
    } else {
        AiQueue_SortDescending(i);
    }
}

#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);
void func_0208f9c4(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by each unit's maximum attack range, smallest first. */
void AiQueue_SortByMaxRange(void)
{
    int i;

    /* needed to match: a plain for condition rotates the loop */
    for (i = 0;; i++) {
        if (gAiQueue[i] == 0) {
            break;
        }
        data_0229e234[i] = GetUnitMaxRange(STATE->currentPlayer, STATE->units[gAiQueue[i]].type);
    }
    func_0208f9c4(i);
}

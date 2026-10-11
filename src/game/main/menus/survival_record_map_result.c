#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7a9c.h"
#include "game/Unk_022a7b80.h"
#include "game/player.h"

BOOL Player1Team_HasPlayerInGame(void);
int Survival_GetMapCost(int mode);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

#define STATE (data_027c027c->unk_48[data_027c0284])

#define POINTS_MAX 9999

/* After a Survival battle: keeps player 1's COs and power meters for the
 * next map. A win advances the run to the next map, adds the battle's
 * unk_14 (BattleResults.expGain) to the run's points, up to 9999, and takes
 * the map's cost from the budget, down to 0. A loss empties the budget. */
void Survival_RecordMapResult(void)
{
    int i;
    u32 cost;

    if (STATE->gameMode != GAME_MODE_SURVIVAL) {
        return;
    }
    for (i = 0; i < 2; i++) {
        data_022a7b80.coIds[i] = (&STATE->players[1].front)[i].id & PLAYER_CO_ID_MASK;
        data_022a7b80.powerCharge[i] = (&STATE->players[1].front)[i].charge;
    }
    if (Player1Team_HasPlayerInGame()) {
        data_022a7b80.mapIndex++;
        /* the original compares unsigned */
        if ((u32)(data_022a7b80.points + data_022a7a9c.totalPoints) < POINTS_MAX) {
            data_022a7b80.points = data_022a7b80.points + data_022a7a9c.totalPoints;
        } else {
            data_022a7b80.points = POINTS_MAX;
        }
        cost = Survival_GetMapCost(data_022a7b80.mode);
        if ((u32)data_022a7b80.budgetLeft <= cost) {
            data_022a7b80.budgetLeft = 0;
        } else {
            data_022a7b80.budgetLeft -= cost;
        }
    } else {
        data_022a7b80.budgetLeft = 0;
    }
}

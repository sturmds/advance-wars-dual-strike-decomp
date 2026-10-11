#include "types.h"
#include "game/co_data.h"
#include "game/play_records.h"

int Co_GetRank(int co);

/* Stores the sum of the ranks of all COs in gPlayRecords. */
void PlayRecords_UpdateCoRankTotal(void)
{
    int co;
    int total = 0;

    for (co = 1; co < CO_COUNT; co++) {
        total += Co_GetRank(co);
    }
    gPlayRecords.coRankTotal = total;
}

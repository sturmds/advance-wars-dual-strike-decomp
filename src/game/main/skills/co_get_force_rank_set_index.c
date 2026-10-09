#include "types.h"
#include "game/Unk_02290718.h"

/* Which of the CO's four saved Force Rank sets (0-3) player (1-4) uses. */
int Co_GetForceRankSetIndex(int co, int player)
{
    return (data_02290718.coRecords[co].forceRankSetByPlayer >> ((player - 1) * 2)) & 3;
}

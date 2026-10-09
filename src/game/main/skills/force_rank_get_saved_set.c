#include "types.h"
#include "game/Unk_02290718.h"

extern ForceRankSet data_02290128[];

/* Saved skill list of set `set` of CO `co` in save mode `mode`, or NULL for
 * an unknown mode. Only mode 2 has more than one set. */
u8 *ForceRank_GetSavedSet(int mode, int co, int set)
{
    switch (mode) {
    case 1:
        return data_02290718.coRecords[co].setMode1;
    case 0:
        return data_02290128[co];
    case 2:
        return data_02290718.coRecords[co].setsMode2[set];
    case 3:
        return data_02290718.coRecords[co].setMode3;
    }
    return NULL;
}

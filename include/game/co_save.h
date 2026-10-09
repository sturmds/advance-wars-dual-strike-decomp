#ifndef GAME_CO_SAVE_H
#define GAME_CO_SAVE_H

#include "types.h"

/* Skill ids of one saved Force Rank set, ended by 0 when fewer than 4. */
typedef u8 ForceRankSet[4];

/* Saved progress of one CO. The records sit in Unk_02290718.coRecords,
 * indexed by CO id. */
typedef struct CoSaveRecord {
    u32 experience : 19;               /* func_020179c0 caps it at 100000 */
    u32 unk_0_19 : 13;
    ForceRankSet setMode1;             /* ForceRank_GetSavedSet mode 1 */
    ForceRankSet setMode3;             /* mode 3 */
    ForceRankSet setsMode2[4];         /* mode 2 */
} CoSaveRecord;

#endif

#ifndef GAME_UNK_02290118_H
#define GAME_UNK_02290118_H

#include "types.h"
#include "game/co_data.h"

/* data_02290118 (main bss): saved progress. Only the part the campaign
 * world map reads is described. */
typedef struct Unk_02290118 {
    u8 unk_00;                         /* picks the Force Rank profile Save_LoadProgress reads from the save buffer */
    u8 pad_001[0x11a - 0x01];
    u8 coScore[CO_COUNT];              /* per CO id; the world map shows the three highest */
} Unk_02290118;

extern Unk_02290118 data_02290118;

#endif

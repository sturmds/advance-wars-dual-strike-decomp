#ifndef GAME_AI_BUILD_STATE_H
#define GAME_AI_BUILD_STATE_H

#include "types.h"

/* Work area at *data_021619c0, used by the AI unit-action-end phase
 * (AI_STATE_END_ACTIONS), which runs AiBuildState.step through ten steps.
 * Only the fields seen so far are listed. */
typedef struct AiBuildState {
    u8 recordCount;                    /* entries in data_0229e5bc, 12 bytes each; the loop bound of func_02092134 */
    u8 slotCount;                      /* entries in data_0229e44c, 4 bytes each; the loop bound of func_020937dc */
    u8 step;                           /* 0-9, advanced by AiTurn_RunEndActions; cleared by func_0208d964 */
    u8 unk_03;                         /* cleared by func_0208d964 */
    u8 pad_04;
    u8 unk_05;                         /* set to 2 by AiEndActions_Start */
    u8 pad_06;
    u8 unk_07;                         /* AiTurn_RunEndActions ends the phase once it reaches 50 after step 0; steps 2 to 5 run only when nonzero */
    u8 unk_08;                         /* set by func_02090154 from func_0209f980(1) */
    u8 pad_09[3];
    s32 unk_0c;                        /* AiEndActions_Start sums func_02092ef4 over the units that Unit_CanRepairAt accepts, then scales it by 3/2 (at least 1000) */
    u32 unk_10;                        /* cleared by AiEndActions_Start */
} AiBuildState;

extern AiBuildState *data_021619c0;

#endif

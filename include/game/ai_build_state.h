#ifndef GAME_AI_BUILD_STATE_H
#define GAME_AI_BUILD_STATE_H

#include "types.h"

/* Work area at *data_021619c0, used by the AI unit-action-end phase
 * (AI_STATE_END_ACTIONS), which runs AiBuildState.step through ten steps.
 * Only the fields seen so far are listed. */
typedef struct AiBuildState {
    u8 pad_00[2];
    u8 step;                           /* 0-9, advanced by AiTurn_RunEndActions; cleared by func_0208d964 */
    u8 unk_03;                         /* cleared by func_0208d964 */
    u8 pad_04[3];
    u8 unk_07;                         /* AiTurn_RunEndActions ends the phase once it reaches 50 after step 0; steps 2 to 5 run only when nonzero */
    u8 unk_08;                         /* set by func_02090154 from func_0209f980(1) */
} AiBuildState;

extern AiBuildState *data_021619c0;

#endif

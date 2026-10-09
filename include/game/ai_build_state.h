#ifndef GAME_AI_BUILD_STATE_H
#define GAME_AI_BUILD_STATE_H

#include "types.h"

/* Work area at *data_021619c0, used by the AI routines that start at
 * func_02090154 (they look like the choice of what to build). Only the
 * fields seen so far are listed. */
typedef struct AiBuildState {
    u8 pad_00[2];
    u8 unk_02;                         /* cleared by func_0208d964 */
    u8 unk_03;                         /* cleared by func_0208d964 */
    u8 pad_04[4];
    u8 unk_08;                         /* set by func_02090154 from func_0209f980(1) */
} AiBuildState;

extern AiBuildState *data_021619c0;

#endif

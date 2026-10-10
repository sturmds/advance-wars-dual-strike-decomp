#ifndef GAME_UNK_OV005_023559DC_H
#define GAME_UNK_OV005_023559DC_H

#include "types.h"

struct Task;

/* Work block of the overlay 5 screens that fade in, run an event script and
 * fade out (Ov005_UpdateEventScriptScreen, func_ov005_02355f80). */
typedef struct Unk_ov005_023559dc {
    u8 pad_00[0x10];
    struct Task *fadeTask;             /* from the func_020fcd78 family */
} Unk_ov005_023559dc;

#endif

#ifndef GAME_UNK_OV005_023559DC_H
#define GAME_UNK_OV005_023559DC_H

#include "types.h"

struct Task;

/* Work block of the overlay 5 screens that fade in, run an event script and
 * fade out (Ov005_UpdateEventScriptScreen, func_ov005_02355f80). */
typedef struct Unk_ov005_023559dc {
    u8 pad_00[0x10];
    struct Task *fadeTask;             /* from the func_020fcd78 family */
    struct Task *fadeTask2;
    struct Task *songFadeTask;         /* func_020fcbd8 or func_020fcb08 */
    u32 scrollFrame;                   /* frame of the BG scroll tween */
    u32 scrollFrames;
} Unk_ov005_023559dc;

#endif

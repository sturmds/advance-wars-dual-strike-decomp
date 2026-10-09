#ifndef GAME_EVENT_SCRIPT_H
#define GAME_EVENT_SCRIPT_H

#include "types.h"

#define EVENT_SCRIPT_SLOTS 10

/* One of 10 running scripts in data_022977f0 (map "Terms" and "Hint"
 * scripts, Hachi tips). A script is a list of 16-byte commands whose first
 * byte indexes a handler table. */
typedef struct EventScriptSlot {
    const void *start;                 /* script start; NULL: slot free */
    const void *pc;                    /* next command */
    u32 hold;                          /* while nonzero the script does not advance */
    u16 wait;                          /* frames left before the next command */
    u8 pad_0e[0x18 - 0x0e];
} EventScriptSlot;

extern EventScriptSlot data_022977f0[EVENT_SCRIPT_SLOTS];

#endif

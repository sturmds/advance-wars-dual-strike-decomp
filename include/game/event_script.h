#ifndef GAME_EVENT_SCRIPT_H
#define GAME_EVENT_SCRIPT_H

#include "types.h"

#define EVENT_SCRIPT_SLOTS 10

struct EventScriptSlot;

/* One 16-byte command of an event script. The opcode indexes a handler
 * table. */
typedef struct EventScriptCommand {
    u8 opcode;
    u8 pad_01[0x03];
    void (*handler)(struct EventScriptSlot *slot); /* EventScript_RunCommand calls it with the running slot */
    u8 pad_08[0x04];
    u32 arg;                           /* wait command: frames to wait */
} EventScriptCommand;

/* One of 10 running scripts in data_022977f0 (the map "Terms" and "Hint"
 * scripts start here). */
typedef struct EventScriptSlot {
    const EventScriptCommand *start;   /* script start; NULL: slot free */
    const EventScriptCommand *pc;      /* next command */
    u32 hold;                          /* while nonzero the script does not advance */
    u16 wait;                          /* frames left before the next command */
    u8 pad_0e[0x18 - 0x0e];
} EventScriptSlot;

extern EventScriptSlot data_022977f0[EVENT_SCRIPT_SLOTS];

#endif

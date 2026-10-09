#ifndef GAME_SCRIPT_SLOT_H
#define GAME_SCRIPT_SLOT_H

#include "types.h"

/* One 8-byte command of a slot script. Handlers in gScriptCommands read
 * the two arguments and advance ScriptSlot.pc. */
typedef struct ScriptCommand {
    u32 arg;                           /* opcode 1: function stored in ScriptSlot.callback */
    u16 arg16;                         /* opcode 0: frames to wait */
    u16 opcode;                        /* index into gScriptCommands */
} ScriptCommand;

/* One of 30 script slots at data_027c027c->scriptSlots. The handlers call
 * float and fixed-point helpers, so the scripts probably animate
 * something. */
typedef struct ScriptSlot {
    ScriptCommand *start;              /* NULL: slot unused */
    ScriptCommand *pc;                 /* next command */
    void (*callback)(void);            /* set by opcode 1; while set, the script does not advance */
    u8 pad_0c[0x10 - 0x0c];
    u16 wait;                          /* frames left before the next command */
    u8 pad_12[0x60 - 0x12];
} ScriptSlot;

/* Runs one command of the slot; nonzero: run the next one this frame too. */
typedef BOOL (*ScriptCommandFunc)(int slot);

#endif

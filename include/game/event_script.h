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

/* One 8-byte entry of a map's trigger list. A list holds blocks of
 * conditions, each block closed by one EVENT_TRIGGER_RUN_* entry, and ends
 * with EVENT_TRIGGER_END. Condition kinds come in threes: the first applies
 * only on the map state at data_02183b08, the second only on the other one,
 * the third on both. */
typedef struct EventTrigger {
    u8 kind;                           /* EVENT_TRIGGER_* */
    u8 param;                          /* game flag, unit type, player slot or id, by kind */
    u16 day;                           /* EVENT_TRIGGER_DAY: 0xffff any day */
    union {
        const EventScriptCommand *script; /* EVENT_TRIGGER_RUN_*: NULL: none */
        BOOL (*test)(void);            /* EVENT_TRIGGER_TEST_* */
    } u;
} EventTrigger;

#define EVENT_TRIGGER_DAY           0  /* day and player slot (0: any player) */
#define EVENT_TRIGGER_UNIT_TYPE     3  /* the unit passed in has type param */
#define EVENT_TRIGGER_FLAG_SET      6  /* game flag param is set */
#define EVENT_TRIGGER_FLAG_CLEAR    9  /* game flag param is clear */
#define EVENT_TRIGGER_ID            12 /* the id passed in equals param */
#define EVENT_TRIGGER_TEST_TRUE     15 /* test() returns 1 */
#define EVENT_TRIGGER_TEST_FALSE    18 /* test() returns 0 */
#define EVENT_TRIGGER_NORMAL_MODE   21 /* campaign not in hard mode */
#define EVENT_TRIGGER_HARD_MODE     22 /* campaign in hard mode */
#define EVENT_TRIGGER_RUN           23 /* run script once; param: game flag that marks it done, 0xff none */
#define EVENT_TRIGGER_END           26

#define EVENT_TRIGGER_NO_DAY  0xffff
#define EVENT_TRIGGER_NO_FLAG 0xff

#endif

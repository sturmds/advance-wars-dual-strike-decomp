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
  void (*handler)(
      struct EventScriptSlot*
          slot); /* EventScript_RunCommand calls it with the running slot */
  s16 unk_08;
  s16 unk_0a;
  u32 arg; /* wait command: frames to wait */
} EventScriptCommand;

/* The same 16 bytes as the data-moving commands read them: the word at +4 is
 * an address or a script instead of a handler. */
typedef struct EventScriptDataCommand {
  u8 opcode;
  u8 pad_01[0x03];
  void* target; /* address the command writes to, or the script it waits for */
  s16 unk_08;
  s16 unk_0a;
  u32 value; /* value to store, or the address to copy from */
} EventScriptDataCommand;

/* One of 10 running scripts in data_022977f0 (the map "Terms" and "Hint"
 * scripts start here). */
typedef struct EventScriptSlot {
  const EventScriptCommand* start; /* script start; NULL: slot free */
  const EventScriptCommand* pc;    /* next command */
  u32 hold; /* hold callback, stored as an address; while nonzero the script does not advance */
  u16 wait; /* frames left before the next command */
  s16 counter; /* step count of the hold function (the screen slide) */
  s8 unk_10;   /* set by the code that starts a script; a few scripts read it */
  u8 player;   /* player slot the hold callback waits for */
  s16 day;     /* day the hold callback waits for; negative: any */
  const EventScriptCommand*
      unk_14; /* set by the event command that starts the script: the starting slot's pc */
} EventScriptSlot;

extern EventScriptSlot data_022977f0[EVENT_SCRIPT_SLOTS];

/* Command at the pc of slot index, for the data-moving commands. */
#define EVENT_SCRIPT_DATA_COMMAND(index) \
  ((const EventScriptDataCommand*)data_022977f0[index].pc)

EventScriptSlot* EventScript_Start(const EventScriptCommand* script);

#define EVENT_SCRIPT_QUEUE_SIZE 8

/* Scripts queued by EventScript_StartOrQueue while another script runs. */
extern const EventScriptCommand* gEventScriptQueue[EVENT_SCRIPT_QUEUE_SIZE];

/* One 8-byte entry of a map's trigger list. A list holds blocks of
 * conditions, each block closed by one EVENT_TRIGGER_RUN_* entry, and ends
 * with EVENT_TRIGGER_END. Condition kinds come in threes: the first applies
 * only on the map state at data_02183b08, the second only on the other one,
 * the third on both. */
typedef struct EventTrigger {
  u8 kind;  /* EVENT_TRIGGER_* */
  u8 param; /* game flag, unit type, player slot or id, by kind */
  u16 day;  /* EVENT_TRIGGER_DAY: 0xffff any day */
  union {
    const EventScriptCommand* script; /* EVENT_TRIGGER_RUN_*: NULL: none */
    BOOL (*test)(void);               /* EVENT_TRIGGER_TEST_* */
  } u;
} EventTrigger;

#define EVENT_TRIGGER_DAY 0          /* day and player slot (0: any player) */
#define EVENT_TRIGGER_UNIT_TYPE 3    /* the unit passed in has type param */
#define EVENT_TRIGGER_FLAG_SET 6     /* game flag param is set */
#define EVENT_TRIGGER_FLAG_CLEAR 9   /* game flag param is clear */
#define EVENT_TRIGGER_ID 12          /* the id passed in equals param */
#define EVENT_TRIGGER_TEST_TRUE 15   /* test() returns 1 */
#define EVENT_TRIGGER_TEST_FALSE 18  /* test() returns 0 */
#define EVENT_TRIGGER_NORMAL_MODE 21 /* campaign not in hard mode */
#define EVENT_TRIGGER_HARD_MODE 22   /* campaign in hard mode */
#define EVENT_TRIGGER_RUN \
  23 /* run script once; param: game flag that marks it done, 0xff none */
#define EVENT_TRIGGER_END 26

#define EVENT_TRIGGER_NO_DAY 0xffff
#define EVENT_TRIGGER_NO_FLAG 0xff

#endif

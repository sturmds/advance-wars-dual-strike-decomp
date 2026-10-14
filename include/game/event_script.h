#ifndef GAME_EVENT_SCRIPT_H
#define GAME_EVENT_SCRIPT_H

#include "types.h"

#define EVENT_SCRIPT_SLOTS 10

struct EventScriptSlot;

/* One 16-byte command of an event script. The opcode (EVENT_OP_*) indexes
 * the handler table data_021585c0; each handler reads the fields it needs
 * and moves the slot's pc on, or jumps. A script ends with EVENT_OP_END (or
 * another opcode with the same handler) or loops back with EVENT_OP_JUMP. */
typedef struct EventScriptCommand {
  u8 opcode; /* EVENT_OP_* */
  u8 pad_01[0x03];
  union {
    /* first, so that a data initializer can set any member */
    const void* ptr;
    /* EVENT_OP_CALL: called with the running slot */
    void (*handler)(struct EventScriptSlot* slot);
    /* EVENT_OP_JUMP and the conditional jumps: where to go on */
    const struct EventScriptCommand* jump;
  } u;
  s16 unk_08; /* by opcode: player, picture, flag, tile x */
  s16 unk_0a; /* by opcode: CO, mood, tile y */
  u32 arg;    /* by opcode: frames to wait, text id, song, test function */
} EventScriptCommand;

typedef char
    EventScriptCommand_size_check[sizeof(EventScriptCommand) == 0x10 ? 1 : -1];

/* Opcodes, named after their handlers in data_021585c0. Opcodes 2, 3 and 22
 * have the same handler as EVENT_OP_END; 13, 35-38, 40 and 61 only move on;
 * 5 and 39 end the frame without moving on. 66, 68 and 69 work like 65, 67
 * and 70 with differences not understood yet. */
#define EVENT_OP_CALL 0       /* u.handler(slot) */
#define EVENT_OP_WAIT 1       /* wait arg frames */
#define EVENT_OP_END 4        /* stop the script */
#define EVENT_OP_STORE_BYTE 6 /* EventScriptDataCommand: *target = value */
#define EVENT_OP_STORE_HALFWORD 7
#define EVENT_OP_STORE_WORD 8
#define EVENT_OP_COPY_BYTE 9 /* EventScriptDataCommand: *target = *value */
#define EVENT_OP_COPY_HALFWORD 10
#define EVENT_OP_COPY_WORD 11
#define EVENT_OP_SET_MAP_ID 12
#define EVENT_OP_CREATE_UNIT 16
#define EVENT_OP_WAIT_WITH_KEYS 17 /* keys unk_08 for arg frames */
#define EVENT_OP_RELEASE_INPUT 18
#define EVENT_OP_TAKE_INPUT 19
#define EVENT_OP_SET_VIEW_TARGET_TILE 20 /* (unk_08, unk_0a) */
#define EVENT_OP_ADD_TO_VIEW_TARGET_TILE 21
#define EVENT_OP_SHOW_PICTURE 23 /* picture unk_08, mood unk_0a, screen arg */
#define EVENT_OP_SLIDE_IN 24
#define EVENT_OP_PRINT_WINDOW_TEXT 25 /* text id arg */
#define EVENT_OP_PRINT_TEXT 26
#define EVENT_OP_SET_HOLD 27 /* hold callback u.ptr */
#define EVENT_OP_WAIT_FOR_KEYS 28
#define EVENT_OP_JUMP 29 /* go on at u.jump */
#define EVENT_OP_JUMP_IF_TRUE \
  30 /* u.jump when the function in arg returns nonzero */
#define EVENT_OP_SET_PLAYER_CONTROLLER 31
#define EVENT_OP_SET_MAP_FLAGS 32
#define EVENT_OP_SET_SECOND_SCREEN_MAP_UNK37 33
#define EVENT_OP_SET_PLAYER_FRONT_CO 34
#define EVENT_OP_START_HELPER_SCRIPT 41
#define EVENT_OP_STOP_HELPER_SCRIPT 42
#define EVENT_OP_WAIT_FOR_ATTACK_TASKS 43
#define EVENT_OP_PRINT_SLOT_TEXT 44
#define EVENT_OP_SET_FLAG84 45
#define EVENT_OP_CLEAR_FLAG84 46
#define EVENT_OP_LOAD_ARMY_PALETTE 49 /* player unk_08; 5: current */
#define EVENT_OP_SET_PALETTE 50       /* army unk_08 */
#define EVENT_OP_LOAD_CURRENT_ARMY_PALETTE 51
#define EVENT_OP_CALL_WITH_ARG 54   /* u.handler called with arg */
#define EVENT_OP_HOLD_UNTIL_TURN 55 /* player unk_08, day unk_0a */
#define EVENT_OP_CHANGE_PICTURE 57  /* picture unk_08 */
#define EVENT_OP_SET_PICTURE 58
#define EVENT_OP_STOP_OTHER_SCRIPTS 59
#define EVENT_OP_HOLD_UNTIL_POWER 60
#define EVENT_OP_ROLL_WEATHER 62
#define EVENT_OP_JUMP_IF_FLAG_SET 63 /* u.jump when game flag unk_08 is set */
#define EVENT_OP_JUMP_IF_FLAG_CLEAR 64
#define EVENT_OP_DEFEAT_OTHER_TEAMS 65 /* player unk_08; 5: current */
#define EVENT_OP_DEFEAT_TEAM 67        /* player unk_08; 5: current */
#define EVENT_OP_PLAY_SE 70            /* sound effect arg */
#define EVENT_OP_PLAY_SONG 71          /* song arg */
#define EVENT_OP_PLAY_CO_THEME 72
#define EVENT_OP_FADE_OUT_SONG_AFTER_REQUEST 73
#define EVENT_OP_FADE_OUT_SONG_HARD_MODE 74
#define EVENT_OP_FADE_OUT_SONG 75
#define EVENT_OP_WAIT_SONG_FADE 76
#define EVENT_OP_SKIP_UNLESS_CO 77 /* skip arg commands unless the CO matches */
#define EVENT_OP_JUMP_IF_COS \
  78                         /* u.jump when player arg has COs unk_08, unk_0a */
#define EVENT_OP_SET_FLAG 79 /* game flag unk_08 */
#define EVENT_OP_CLEAR_FLAG 80
#define EVENT_OP_SCROLL_MAP_AND_WAIT 81
#define EVENT_OP_CALL_UNLESS_SKIPPING 82 /* EVENT_OP_CALL unless skipping */
#define EVENT_OP_WAIT_UNLESS_SKIPPING 83 /* EVENT_OP_WAIT unless skipping */
#define EVENT_OP_START_TASK 84           /* task script u.ptr */
#define EVENT_OP_WAIT_TASK 85
#define EVENT_OP_WAIT_TASK_SCREEN0 86
#define EVENT_OP_WAIT_TASK_SCREEN1 87
#define EVENT_OP_WAIT_TASK_BOTH_SCREENS 88
#define EVENT_OP_REQUEST_SCENE_MODE 89 /* mode unk_08 */
#define EVENT_OP_SET_TIME_LIMIT 90
#define EVENT_OP_CLEAR_TIME_LIMIT 91
#define EVENT_OP_START_TEXT_MAP_TASK 92
#define EVENT_OP_COUNT 93 /* entries of data_021585c0 */

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

typedef char EventTrigger_size_check[sizeof(EventTrigger) == 8 ? 1 : -1];

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

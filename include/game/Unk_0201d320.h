#ifndef GAME_UNK_0201D320_H
#define GAME_UNK_0201D320_H

#include "types.h"

/* 16-byte record; the handlers step through an array of these. */
typedef struct Unk_0201d320_Entry {
  u8 pad_00[0x4];
  union {
    struct Unk_0201d320_Entry* jump; /* the record to continue at */
    void (
        *run)(); /* function to call; unprototyped: called with 0 or 1 argument */
  } unk_04;
  s16 unk_08;
  s16 unk_0a;
  u32 unk_0c; /* key mask, argument for unk_04.run, or a wait function (returns 0 when done) */
} Unk_0201d320_Entry;

/* Element of data_022977f4 (24 bytes each). */
typedef struct Unk_0201d320 {
  Unk_0201d320_Entry* unk_00; /* current record */
  u8 pad_04[0x4];
  u16 unk_08; /* wait */
  u8 pad_0a[0xe];
} Unk_0201d320;

/* Unk_02080440_48.unk_18 value that the wait commands and hold callbacks test; meaning unknown. */
#define EVENT_GAME_STATE_14 14

/* The hold callback, player and day of an event script slot (EventScriptSlot
 * hold, player and day) as the commands address them: three more 24-byte
 * strided arrays, starting at data_022977f8, data_02297801 and
 * data_02297802. */
typedef struct EventSlotHold {
  u32 hold;
  u8 pad_04[0x14];
} EventSlotHold;
typedef struct EventSlotPlayer {
  u8 player;
  u8 pad_01[0x17];
} EventSlotPlayer;
typedef struct EventSlotDay {
  s16 day;
  u8 pad_02[0x16];
} EventSlotDay;
typedef struct EventSlotCounter {
  s16 counter;
  u8 pad_02[0x16];
} EventSlotCounter;
/* The wait of an event script slot, starting at data_022977fc. */
typedef struct EventSlotWait {
  u16 wait;
  u8 pad_02[0x16];
} EventSlotWait;
/* The text id of an event script slot, starting at data_02297804. */
typedef struct EventSlotText {
  u32 textId;
  u8 pad_04[0x14];
} EventSlotText;

#endif

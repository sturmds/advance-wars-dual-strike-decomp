#ifndef GAME_NAME_ENTRY_WORK_H
#define GAME_NAME_ENTRY_WORK_H

#include "types.h"

struct SpriteAnim;

#define NAME_ENTRY_KEY_COUNT 75        /* cursor values from this on select a control key */
#define NAME_ENTRY_CONTROL_KEY_COUNT 3 /* kept in unk_4b8, right after keys */
#define NAME_ENTRY_KEY_BUTTON_ID 128   /* menu button id of key 0 */

/* One key of the name entry screen. unk_00 is 0 for an unused slot. */
typedef struct NameEntryKey {
    s16 unk_00;
    s16 buttonX;                       /* screen position of its menu button */
    s16 buttonY;
    s16 x;
    s16 y;
    s16 buttonId;                      /* menu button id: 128 + key index */
} NameEntryKey;

/* Work block of the name entry screen (0x4f4 bytes, allocated by
 * NameEntry_AllocWork); gNameEntry points to it. The text being typed is
 * gNameEntryText. */
typedef struct NameEntry {
    int maxLength;                     /* 8 or 16: width of the text field in characters */
    u16 frameTiles[4][32];             /* tile map of the text field frame: left edge, middle, right edge in each row */

    int cursor;                        /* index into keys; NAME_ENTRY_KEY_COUNT and above: a control key */
    int lastColumn;                    /* keyboard column that up and down from the control keys return to; -1: none */
    int prevCursor;                    /* cursor of the last frame: a move plays SE 0x10002 */
    int textLength;                    /* characters typed so far */
    int unk_114;                       /* text length when the screen opened */
    u32 scriptRunning;                 /* 1 from NameEntry_StartScript until the script runs NameEntry_EndScript */
    int unk_11c;
    int unk_120;                       /* set to 1 when control key 2 is confirmed */
    int unk_124;                       /* set to 1 when control key 0 is confirmed */
    int unk_128;                       /* set to 1 when control key 1 is confirmed */
    int unk_12c;
    int unk_130;
    NameEntryKey keys[NAME_ENTRY_KEY_COUNT];
    u8 unk_4b8[0x24];
    void *spriteFile;                  /* loaded sprite file */
    struct SpriteAnim *caret;        /* text caret sprite */
    void *buttonSpriteFile;          /* loaded sprite file the buttonSprites come from */
    struct SpriteAnim *buttonSprites[3];
} NameEntry;

/* Task of the name entry screen. The first 0x2c bytes belong to the task
 * system (Task_Create). */
typedef struct NameEntryTask {
    u8 pad_00[0x2c];
    NameEntry *work;
    u8 pad_30[0x34 - 0x30];
    int step;
    u8 pad_38[0x54 - 0x38];
    int maxLength;                     /* copied to NameEntry.maxLength */
} NameEntryTask;

#endif

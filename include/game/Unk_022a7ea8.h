#ifndef GAME_UNK_022A7EA8_H
#define GAME_UNK_022A7EA8_H

#include "types.h"

struct Unk_02022404;

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
 * NameEntry_AllocWork); data_022a7ea8 points to it. The text being typed is
 * data_022a7ed0. */
typedef struct Unk_022a7ea8 {
    int maxLength;                     /* 8 or 16: width of the text field in characters */
    u8 pad_04[0x104 - 0x04];
    int cursor;                        /* index into keys; NAME_ENTRY_KEY_COUNT and above: a control key */
    int unk_108;                       /* set to -1 when a control key is selected other than by up or down */
    u8 pad_10c[0x110 - 0x10c];
    int textLength;                    /* characters typed so far */
    u8 pad_114[0x118 - 0x114];
    u32 unk_118;
    int unk_11c;
    int unk_120;                       /* set to 1 when control key 2 is confirmed */
    int unk_124;                       /* set to 1 when control key 0 is confirmed */
    int unk_128;                       /* set to 1 when control key 1 is confirmed */
    u8 pad_12c[0x134 - 0x12c];
    NameEntryKey keys[NAME_ENTRY_KEY_COUNT];
    u8 unk_4b8[0x24];
    void *spriteFile;                  /* loaded sprite file */
    struct Unk_02022404 *caret;        /* text caret sprite */
    void *buttonSpriteFile;          /* loaded sprite file the buttonSprites come from */
    struct Unk_02022404 *buttonSprites[3];
} Unk_022a7ea8;

typedef Unk_022a7ea8 NameEntry;

/* Task of the name entry screen. The first 0x2c bytes belong to the task
 * system (Task_Create). */
typedef struct NameEntryTask {
    u8 pad_00[0x2c];
    NameEntry *work;
    u8 pad_30[0x34 - 0x30];
    int step;
} NameEntryTask;

#endif

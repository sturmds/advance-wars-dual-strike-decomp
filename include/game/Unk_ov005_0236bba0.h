#ifndef GAME_UNK_OV005_0236BBA0_H
#define GAME_UNK_OV005_0236BBA0_H

#include "types.h"

/* Overlay 5 work block (0x918 bytes) that Ov005_AllocCoListWork allocates
 * into Ov005Task.work and data_ov005_0236bba0. It belongs to a screen with
 * 33 selectable entries (the rows of the table at data_ov005_0236a5a0), one
 * menu button per entry. */
typedef struct Unk_ov005_0236bba0 {
    u8 pad_00[0x82c];
    int unk_82c;                       /* 0x1000 or -0x1000, set from the key state */
    int unk_830;                       /* nonzero while the button with id 243 exists */
    int unk_834;                       /* nonzero while the button with id 244 exists */
    int unk_838;
    int unk_83c;
    u8 pad_840[0x850 - 0x840];
    u8 buttonIds[33];                  /* MenuButton id of each entry */
    u8 entryActive[33];                /* nonzero when the entry has a button */
    u8 pad_892[0x918 - 0x892];
} Unk_ov005_0236bba0;

#endif

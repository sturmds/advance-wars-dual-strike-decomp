#ifndef GAME_UNK_OV005_0236BBA0_H
#define GAME_UNK_OV005_0236BBA0_H

#include "types.h"

struct Task;

/* Overlay 5 work block (0x918 bytes) that Ov005_AllocCoListWork allocates
 * into Ov005Task.work and data_ov005_0236bba0. It belongs to a screen with
 * 33 selectable entries (the rows of the table at data_ov005_0236a5a0), one
 * menu button per entry. */
typedef struct Unk_ov005_0236bba0 {
  u8 pad_00[0x04];
  int unk_04;
  int unk_08;
  int unk_0c;
  int unk_10;
  u8 pad_14[0x18 - 0x14];
  struct Task*
      fadeTask; /* palette fade (PaletteFade_StartToBlackScreen1, PaletteFade_StartFromBlackScreen1) */
  u8 pad_1c[0x41c - 0x1c];
  u8 savedPalettes[0x400]; /* copy of the sub screen's paletteBuffer */
  u8 pad_81c[0x824 - 0x81c];
  int scrollX; /* layer 3 scroll, 20.12 fixed point */
  int scrollY;
  int unk_82c;        /* 0x1000 or -0x1000, set from the key state */
  int button243Added; /* nonzero while the button with id 243 exists */
  int button244Added; /* nonzero while the button with id 244 exists */
  int selectedEntry;  /* index of the entry whose button was selected last */
  int cursorRow;      /* row kept when the cursor moves left or right */
  int unk_840;
  int unk_844;
  int unk_848;
  int unk_84c;
  u8 buttonIds[33];   /* MenuButton id of each entry */
  u8 entryActive[33]; /* nonzero when the entry has a button */
  u8 pad_892[0x894 - 0x892];
  int nameSlots[33]; /* what Text_PrintId returned for each entry's name */
} Unk_ov005_0236bba0;

#endif

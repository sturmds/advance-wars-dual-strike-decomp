#ifndef GAME_UNK_OV005_0236BB90_H
#define GAME_UNK_OV005_0236BB90_H

#include "types.h"
#include "game/task.h"

/* One of the 24-byte entries in the large overlay 5 work block: one text
 * line of a list. A first halfword of 0 ends a list of entries. */
typedef struct Ov005SlideEntry {
  u16 id;
  u16 textLength; /* strlen of text */
  const char* text;
  s16 unk_08;  /* 64 times the line's slot */
  s16 kind;    /* Ov005MenuItem.kind; 2 makes the line 8 pixels taller */
  u16 angle;   /* eased by Ov005Entry_WaitThenTurn */
  s16 offsetX; /* minus half the text width, 4 pixels per character */
  s16 pos;     /* moved by the slide tasks */
  s16 unk_12;
  Task* slideTask; /* task that moves pos */
} Ov005SlideEntry;

#define OV005_SLIDE_ENTRIES 17

/* Overlay 5 work block (0x97e0 bytes) that Ov005Screen_AllocLargeWork
 * allocates into Ov005Task.work and data_ov005_0236bb90. Unk_ov005_0235a30c
 * and the palette data at 0x8360 probably belong to the same block. */
typedef struct Unk_ov005_0236bb90 {
  u8 pad_0000[0x4];
  Task* fadeTask; /* from the func_020fcdc8 family */
  Task* fadeTask2;
  u8 pad_000c[0x0010 - 0x000c];
  u8 buffer_0010[0x2000]; /* see Unk_ov005_0235b574 */
  u8 pad_2010[0x8014 - 0x2010];
  u32 unk_8014; /* set by Ov005_LoadCreditsPage from the table it reads */
  Ov005SlideEntry entriesA[OV005_SLIDE_ENTRIES];
  Ov005SlideEntry entriesB[OV005_SLIDE_ENTRIES];
  Task* unk_8348; /* started by Ov005Screen_LoadPalettes */
  u32 unk_834c;   /* 64 at the start */
  s32 unk_8350;
  s32 unk_8354;
  s32 unk_8358;
  s32 unk_835c;
  u8 unk_8360
      [0x83e0 -
       0x8360]; /* loaded from a file; Ov005Screen_StartPaletteTask passes it on */
  u8 palettes[4][8][64];    /* 8 chunks of 64 bytes per set */
  u8 palettesOut[8][6][64]; /* chunk k of a set is copied to [k][column] */
} Unk_ov005_0236bb90;

void Ov005Work_CopyPaletteSet(Unk_ov005_0236bb90* work, int set, int column);

/* One credits page, as data_ov005_0236a418 lists them: up to
 * OV005_MENU_ITEMS lines (Ov005_LoadCreditsPage). */
#define OV005_MENU_ITEMS 6

typedef struct Ov005MenuItem {
  u32 kind; /* 0: no line */
  const char* text;
} Ov005MenuItem;

typedef struct Ov005Menu {
  Ov005MenuItem items[OV005_MENU_ITEMS];
  u32 unk_30;
} Ov005Menu;

typedef char Ov005Menu_size_check[sizeof(Ov005Menu) == 0x34 ? 1 : -1];

#endif

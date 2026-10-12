#ifndef GAME_UNK_0201A774_H
#define GAME_UNK_0201A774_H

#include "types.h"

/* A two-choice prompt in the event text window (the cursor is a pair of
 * tiles beside the chosen text). */
typedef struct Unk_0201a774 {
  u8 pad_00[0x08];
  void* unk_08; /* TextBox_UpdateSkipInput clears it when the wait ends */
  u8 pad_0c[0x18 - 0x0c];
  u16 (*tiles)[32]; /* tile map of the prompt, 32 entries per row */
  u8 pad_1c[0x1e - 0x1c];
  s16 cursor; /* 0 or 1: the chosen text */
  u8 pad_20[0x28 - 0x20];
  void* text;           /* passed to func_0201d730 when a choice is confirmed */
  void (*redraw)(void); /* called after the cursor is drawn */
  u16 palette;          /* palette slot << 12 (tile map entry bits) */
} Unk_0201a774;

#endif

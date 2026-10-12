#ifndef GAME_UNK_022977A4_H
#define GAME_UNK_022977A4_H

#include "types.h"

/* data_022977a4 (main bss, 12 bytes), state of the event script engine.
 * EventScript_SetArmyPalette passes unk_00 to Gfx_LoadBmap0a0Palette as the palette
 * slot and stores its army colour in armyColour. */
typedef struct Unk_022977a4 {
  u8 unk_00;
  u8 armyColour; /* 0x01: army of the speaker; 5 Black Hole */
  u16 unk_02;
  u16 unk_04;
  u8 pad_06[0x08 - 0x06];
  u32 unk_08;
} Unk_022977a4;

extern Unk_022977a4 data_022977a4;

#endif

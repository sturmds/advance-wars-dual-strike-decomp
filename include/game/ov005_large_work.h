#ifndef GAME_OV005_LARGE_WORK_H
#define GAME_OV005_LARGE_WORK_H

#include "types.h"

/* Work block of the overlay 5 screen that Ov005Screen_AllocLargeWork
 * allocates into Ov005Task.work and data_ov005_0236bb90. Only the palette
 * data is known. */
typedef struct Ov005LargeWork {
  u8 unk_0000[0x8360];
  u8 palettes[0x97e0 - 0x8360]; /* Ov005Screen_StartPaletteTask fades these */
} Ov005LargeWork;

extern Ov005LargeWork* data_ov005_0236bb90;

#endif

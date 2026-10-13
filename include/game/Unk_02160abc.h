#ifndef GAME_UNK_02160ABC_H
#define GAME_UNK_02160ABC_H

#include "types.h"

/* Pointed to by data_02160abc (main data). */
typedef struct Unk_02160abc {
  u8 unk_00
      [10];  /* Ov003Row.unk_28 of the row that Ov003_RecordWinner was given */
  u8 unk_0a; /* Ov003_Update clears it before quitting from the pause menu; Ov003_RecordWinner stores the row number (1 to 5) */
  u8 unk_0b; /* Ov003Row.co of that row */
  u8 unk_0c; /* the same for the other row (2 if the row is 1, else 1) */
  u8 unk_0d; /* Ov003Row.color of the row */
  u8 unk_0e; /* the same for the other row */
} Unk_02160abc;

extern Unk_02160abc* data_02160abc;

#endif

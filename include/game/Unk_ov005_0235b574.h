#ifndef GAME_UNK_OV005_0235B574_H
#define GAME_UNK_OV005_0235B574_H

#include "types.h"

/* Bitmap work block of the screen step Ov005_SetUpStaffCreditsScreen: three pixel
 * buffers, copied between by Ov005_DrawCreditsEntryText and its callees. */
typedef struct Unk_ov005_0235b574 {
  u8 pad_0000[0x10];
  u8 buffer_0010[0x2000];
  u8 buffer_2010[0x4000];
  u8 buffer_6010[0x2000]; /* uploaded to VRAM by Ov005_UploadTileBuffer */
} Unk_ov005_0235b574;

#endif

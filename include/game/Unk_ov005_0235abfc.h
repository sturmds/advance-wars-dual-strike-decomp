#ifndef GAME_UNK_OV005_0235ABFC_H
#define GAME_UNK_OV005_0235ABFC_H

#include "types.h"
#include "game/ov005_task.h"

/* One 0x18-byte entry of the two tables in the work block of the task step
 * Ov005_UpdateStaffCreditsScreen (at offsets 0x8018 and 0x81b0). A table ends at the
 * first entry whose `id` is 0. Each entry owns one task. */
typedef struct Unk_ov005_0235abfc {
  u16 id;
  u16 length; /* characters in `text` */
  const u8*
      text;  /* character codes, mapped to glyphs by Ov005_CharToGlyphIndex */
  u16 tile;  /* first tile of the entry's sprites */
  s16 style; /* 2 and 3 pick the second glyph set */
  u16 angle; /* turn about the Y axis while the entry is drawn in 3D; index into data_0214d4c4 after >> 4 */
  s16 x;
  s16 y;
  u8 pad_12[0x14 - 0x12];
  Ov005Task* task;
} Unk_ov005_0235abfc;

#endif

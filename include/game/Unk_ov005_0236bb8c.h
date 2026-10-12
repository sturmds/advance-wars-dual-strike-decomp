#ifndef GAME_UNK_OV005_0236BB8C_H
#define GAME_UNK_OV005_0236BB8C_H

#include "types.h"

struct Task;
struct TextPrinter;

/* Overlay 5 work block (0x18 bytes) that func_ov005_02358928 allocates
 * into Ov005Task.work and data_ov005_0236bb8c. */
typedef struct Unk_ov005_0236bb8c {
  u8 pad_00[0x4];
  int confirmed; /* set to 1 by the button's onConfirm, Ov005MessageScreen_OnButtonConfirm */
  struct TextPrinter* textPrinter; /* from Text_PrintByIdOnLayer0 */
  struct Task* fadeTask;           /* from the func_020fcdc8 family */
  struct Task* fadeTask2;
  struct Task* songTask; /* song change or fade-out task (func_020fcb08) */
} Unk_ov005_0236bb8c;

#endif

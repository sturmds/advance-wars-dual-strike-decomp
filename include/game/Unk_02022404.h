#ifndef GAME_UNK_02022404_H
#define GAME_UNK_02022404_H

#include "types.h"

/* An object that func_02022404(object, x, y) places. It does nothing when
 * the first word is 0. */
typedef struct Unk_02022404 {
    u32 unk_00;
    u8 pad_04[0x24 - 0x04];
    u32 unk_24;
} Unk_02022404;

void func_02022404(Unk_02022404 *object, int x, int y);

#endif

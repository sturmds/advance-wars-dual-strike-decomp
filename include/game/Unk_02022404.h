#ifndef GAME_UNK_02022404_H
#define GAME_UNK_02022404_H

#include "types.h"

/* An object that func_02022404(object, x, y) places. It does nothing when
 * the first word is 0. */
typedef struct Unk_02022404 {
    u32 unk_00;
    u8 pad_04[0x18 - 0x04];
    u16 unk_18;
    u16 unk_1a;
    u8 pad_1c[0x24 - 0x1c];
    u32 unk_24;
    u32 unk_28;
} Unk_02022404;

void func_02022404(Unk_02022404 *object, int x, int y);

#endif

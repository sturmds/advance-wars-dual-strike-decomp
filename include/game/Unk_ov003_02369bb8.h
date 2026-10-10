#ifndef GAME_UNK_OV003_02369BB8_H
#define GAME_UNK_OV003_02369BB8_H

#include "types.h"

/* One of four 12-byte entries at data_ov003_02369bb8 (overlay 3), found by
 * tile with func_ov003_0235e8fc. */
typedef struct Unk_ov003_02369bb8 {
    u8 pad_00[0x09];
    u8 unk_09_0 : 6;
    u8 unk_09_6 : 2;                   /* not 0: its map tiles use BG palette 13 */
    u8 pad_0a[0x0c - 0x0a];
} Unk_ov003_02369bb8;

#endif

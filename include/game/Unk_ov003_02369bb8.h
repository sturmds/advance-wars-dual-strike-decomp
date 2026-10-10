#ifndef GAME_UNK_OV003_02369BB8_H
#define GAME_UNK_OV003_02369BB8_H

#include "types.h"

/* One of four 12-byte entries at data_ov003_02369bb8 (overlay 3), found by
 * tile with func_ov003_0235e8fc. */
typedef struct Unk_ov003_02369bb8 {
    struct Ov003Unit *target;          /* unit it follows; NULL when none */
    s16 x;                             /* tile column of terrain 21 or 26 (Ov003_LoadMap) */
    s16 y;
    u8 unk_08_0 : 4;                   /* 6 on terrain 21, 7 on 26 */
    u8 unk_08_4 : 3;                   /* row of the tile */
    u8 busy : 1;                       /* set while the entry is in use */
    u8 unk_09_0 : 1;
    u8 unk_09_1 : 5;
    u8 unk_09_6 : 2;                   /* not 0: its map tiles use BG palette 13 */
    u8 unk_0a;
    u8 unk_0b;
} Unk_ov003_02369bb8;

typedef char Unk_ov003_02369bb8_size_check[sizeof(Unk_ov003_02369bb8) == 12 ? 1 : -1];

#endif

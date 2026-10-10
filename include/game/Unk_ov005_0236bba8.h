#ifndef GAME_UNK_OV005_0236BBA8_H
#define GAME_UNK_OV005_0236BBA8_H

#include "types.h"

/* CO picture screen work block (0x2f4 bytes) that func_ov005_02362618
 * allocates into Ov005Task.work and data_ov005_0236bba8. */
typedef struct Unk_ov005_0236bba8 {
    u8 pad_00[0x20];
    u32 coId;                          /* low 7 bits: CO shown */
    u8 pad_24[0x20c - 0x24];
    int unk_20c;                       /* palette variant of the CO */
    int unk_210;                       /* same value */
    int unk_214;                       /* palette variant, 0 when it is 8 */
    u8 pad_218[0x21c - 0x218];
    void *unk_21c[9];
} Unk_ov005_0236bba8;

#endif

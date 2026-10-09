#ifndef GAME_UNK_02290718_H
#define GAME_UNK_02290718_H

#include "types.h"

/* data_02290718 (main bss), 0x38 bytes before the Force Rank table
 * that func_02017abc reads. */
typedef struct Unk_02290718 {
    u8 pad_00[0x12];
    u8 unk_12;                         /* copied to Unk_02066818.unk_2e1 */
    u8 pad_13[0x15 - 0x13];
    u8 unk_15;                         /* copied to Unk_02066818.unk_2e2 */
} Unk_02290718;

#endif

#ifndef GAME_UNK_0215FE84_H
#define GAME_UNK_0215FE84_H

#include "types.h"

/* Pointed to by data_0215fe84 (main data). */
typedef struct Unk_0215fe84 {
    u16 childAids;                     /* bit n set while child aid n is connected */
    u8 pad_02[0x06 - 0x02];
    u8 unk_06;
    u8 pad_07[0x14 - 0x07];
    int unk_14;                        /* set to 1 by Wireless_OnEvent */
    int unk_18;                        /* returned by func_02077f20 */
    int childrenChanged;               /* set to 1 when a child connects or leaves */
    int unk_20;                        /* set to 1 by func_02079180 */
    u8 pad_24[0x30 - 0x24];
    int unk_30;                        /* returned by func_0207a170 */
    u8 unk_34;                         /* returned by func_0207a3a4 */
    u8 unk_35;                         /* set by func_0207a7a4 */
} Unk_0215fe84;

extern Unk_0215fe84 *data_0215fe84;

#endif

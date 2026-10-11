#ifndef GAME_UNK_0229D8E0_H
#define GAME_UNK_0229D8E0_H

#include "types.h"
#include "game/unit_action.h"

/* data_0229d8e0 (main bss): ring buffer of unit actions received over the
 * link. */
typedef struct Unk_0229d8e0 {
    UnitAction slots[16];
    u8 readIndex;
    u8 writeIndex;
} Unk_0229d8e0;

extern Unk_0229d8e0 data_0229d8e0;

#endif

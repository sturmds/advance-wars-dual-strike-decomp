#include "types.h"
#include "game/ai_build_state.h"

/* 4 bytes, count in AiBuildState.slotCount. */
typedef struct Unk_0229e44c {
    u8 pad_00;
    u8 unk_01;
    u8 pad_02[2];
} Unk_0229e44c;

extern Unk_0229e44c data_0229e44c[];

/* Clears Unk_0229e44c.unk_01 in every used slot. Purpose unknown. */
void func_020937dc(void)
{
    int i;
    Unk_0229e44c *slot = data_0229e44c;

    for (i = 0; i < data_021619c0->slotCount; i++, slot++) {
        slot->unk_01 = 0;
    }
}

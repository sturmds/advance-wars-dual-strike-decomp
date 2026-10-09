#include "types.h"
#include "game/ai_build_state.h"

/* 12 bytes, count in AiBuildState.recordCount. */
typedef struct Unk_0229e5bc {
    u8 pad_00[3];
    u8 unk_03;
    u8 pad_04[8];
} Unk_0229e5bc;

extern Unk_0229e5bc data_0229e5bc[];

/* Resets each Unk_0229e5bc.unk_03 of 254 or more to 127. Purpose unknown. */
void func_0209378c(void)
{
    int i;
    Unk_0229e5bc *target = data_0229e5bc;

    for (i = 0; i < data_021619c0->recordCount; i++, target++) {
        if (target->unk_03 >= 254) {
            target->unk_03 = 127;
        }
    }
}

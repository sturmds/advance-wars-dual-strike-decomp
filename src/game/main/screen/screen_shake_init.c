#include "types.h"
#include "game/Unk_02003f44.h"

extern u16 data_0216e2a4;
extern u16 data_0216e2ac;

void ScreenShake_Init(Unk_02003f44 *seq) {
    data_0216e2ac = 0;
    data_0216e2a4 = 0;
    seq->unk_54 = 0;
    seq->unk_58 = 0;
}

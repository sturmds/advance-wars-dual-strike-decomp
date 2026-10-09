#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "game/Unk_0229c960.h"

extern u8 data_0229c9a0[];

void func_0200c054(void *buffer);

void func_0207a750(void)
{
    func_0200c054(data_0229c9a0);
    data_0229c910.unk_14 = 1;
    data_0229c910.unk_16 = data_0229c960.unk_10;
    data_0229c910.unk_17 = data_0215fe84->unk_35;
}

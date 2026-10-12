#include "types.h"
#include "sdk/wm.h"

void func_arm7_027c05ec(Unk_arm7_038057d4 *params, Unk_arm7_038057d4_2 *params2);

/* A 0x600-byte stack; the library gets its top. Written as base + size because the original has two pool entries for 0x0380a874. */
extern u8 data_arm7_0380a274[];
extern u8 data_arm7_0380a874[];
extern u8 data_arm7_0380a0b4[];

/* Fills in the wireless library's init parameters and starts it; arg comes from the ARM7 main. */
void func_arm7_038057d4(u32 arg)
{
    Unk_arm7_038057d4 params;
    Unk_arm7_038057d4_2 params2;

    /* needed to match: the stores stay in the original's order */
    params.unk_00 = data_arm7_0380a274 + 0x600;
    params.unk_04 = data_arm7_0380a874;
    params.unk_08 = 0x600;
    params.unk_0c = 4;
    params.unk_20 = 0;
    params.unk_24 = 8;
    params.unk_28 = arg;
    params.unk_2c = data_arm7_0380a0b4;
    params.unk_30 = 0x1c0;
    params2.unk_00 = 3;
    params.unk_1c = 0x40;
    params2.unk_08 = 3;
    params2.unk_18 = 4;
    params2.unk_10 = 5;
    params2.unk_04 = 7;
    params2.unk_14 = 8;
    params2.unk_0c = 9;
    func_arm7_027c05ec(&params, &params2);
}

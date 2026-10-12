#include "types.h"
#include "sdk/mi.h"

/* Fills size bytes at dest with a 16-bit pattern (hand-written assembly). */
void func_arm7_038057d4(u32 arg);

extern u8 data_arm7_0380a030;
extern u32 data_arm7_0380a034;
extern u32 data_arm7_0380a038;
extern u8 data_arm7_0380a03c[0x78];

/* Called once from the ARM7 main: resets this module's state, then starts the wireless library. */
void func_arm7_038058b0(u32 arg)
{
    data_arm7_0380a038 = 0;
    data_arm7_0380a034 = arg;
    Mem_FillBytes_Arm7(data_arm7_0380a03c, 0, sizeof(data_arm7_0380a03c));
    func_arm7_038057d4(arg);
    data_arm7_0380a030 = 3;
}

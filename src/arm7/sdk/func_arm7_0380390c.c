#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/snd_hw.h"

void func_arm7_03805878(void);

void func_arm7_0380390c(void)
{
    SndHw_Disable();
    func_arm7_03805878();
    Cpu_HaltForever_Arm7();
}

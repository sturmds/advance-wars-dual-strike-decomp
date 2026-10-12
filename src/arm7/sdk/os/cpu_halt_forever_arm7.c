#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037f8444(void);

void Cpu_HaltForever_Arm7(void)
{
    while (TRUE) {
        Cpu_MaskIrq_Arm7();
        func_arm7_037f8444();
    }
}

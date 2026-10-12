#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

void func_arm7_038067bc(void);   /* stops the ARM7; ends in OS_Terminate */

void Cartridge_OnShutdownMessage(u32 tag, u32 data, int err)
{
    if ((data & GBA_SLOT_MSG_OPCODE_BITS) == CTRDG_PXI_COMMAND_TERMINATE) {
        func_arm7_038067bc();
    } else {
        Cpu_HaltForever_Arm7();
    }
}

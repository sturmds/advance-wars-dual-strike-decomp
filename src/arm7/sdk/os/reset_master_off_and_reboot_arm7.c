#include "sdk/os_common.h"

void Reset_HandshakeAndReboot_Arm7(void);

void Reset_MasterOffAndReboot_Arm7(void)
{
    *REG_IME = 0;
    Reset_HandshakeAndReboot_Arm7();
}

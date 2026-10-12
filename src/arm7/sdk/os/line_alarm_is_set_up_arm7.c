#include "types.h"

extern u16 data_arm7_03807a98;

BOOL LineAlarm_IsSetUp_Arm7(void)
{
    return data_arm7_03807a98;
}

#include "sdk/snd.h"

void func_arm7_037fc4e0(OSMessageQueue *queue, int buffer, int count); /* the definition types the buffer as int */
void func_arm7_0380239c(void);

void SndArm7_Init(void)
{
    func_arm7_037fc4e0(&data_arm7_03809210, (int)data_arm7_03809224, 8);
    func_arm7_0380239c();
    data_arm7_0380808c = NULL;
}

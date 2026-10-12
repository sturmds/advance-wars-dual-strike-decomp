#include "types.h"
#include "sdk/os_arm7.h"

void Arena_ApplyDefaults_Arm7(void);
void func_arm7_037fdcf4(void);
void Tick_StartTimer0_Arm7(void);
void Alarm_SetUpTimer1Queue_Arm7(void);
void func_arm7_037fe0a0(void);
void func_arm7_037fb7f0(void);
void func_arm7_037fbabc(void);
void func_arm7_03806ae8(void);

void func_arm7_037fc6c4(void)
{
    Arena_ApplyDefaults_Arm7();
    func_arm7_037fe0a0();
    func_arm7_037fbabc();
    func_arm7_037fb7f0();
    Tick_StartTimer0_Arm7();
    Alarm_SetUpTimer1Queue_Arm7();
    Thread_SetUpTableOnce_Arm7();
    func_arm7_037fdcf4();
    func_arm7_03806ae8();
}

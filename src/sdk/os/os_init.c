#include "types.h"
#include "sdk/os.h"

void func_0210e530(void);
void func_02110e58(void);
void func_0210d21c(void);
void func_0210ced0(void);
void func_0210fa9c(void);
void OS_InitThread(void);
void func_0211e8a0(void);
void func_02115fe8(void);
void func_02115258(void);
void func_0210e4bc(void);
void OS_InitException(void);
void OS_InitVAlarm(void);

/* Runs the OS library initialisers in order. */
void OS_Init(void)
{
    func_0210e530();
    func_02110e58();
    func_0210d21c();
    func_0210e4bc();
    func_0210ced0();
    OS_InitException();
    func_0210fa9c();
    OS_InitVAlarm();
    OSi_InitVramExclusive();
    OS_InitThread();
    OS_InitReset();
    func_0211e8a0();
    func_02115fe8();
    func_02115258();
}

#include "types.h"
#include "sdk/gx.h"

void OS_Terminate(void);
void func_0210e0d4(void);
void func_0210e154(void);
void func_0210eb9c(u32 arg);

#define REG_EXMEMCNT ((volatile u16 *)0x04000204)
#define EXMEMCNT_BIT15 0x8000       /* guess: main memory interface priority */
#define POWCNT1_ENGINES_OFF 0x020e  /* guess: 3D and 2D engine and geometry power bits */
#define MASTER_BRIGHT_WHITE 16

#define SHARED_MEM_WORD (*(volatile u32 *)0x027ffc20)  /* meaning unknown: a word in the shared region at the top of main RAM */

/* Whites out both screens, powers the engines down and ends the program. */
void MainLoop_Shutdown(void)
{
    *REG_POWCNT1 &= ~POWCNT1_ENGINES_OFF;
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, MASTER_BRIGHT_WHITE);
    GXx_SetMasterBrightness_(IO_SUB_SCREEN_FADE, MASTER_BRIGHT_WHITE);
    *REG_EXMEMCNT &= ~EXMEMCNT_BIT15;
    func_0210e0d4();
    func_0210e154();
    func_0210eb9c(SHARED_MEM_WORD + 1);
    OS_Terminate();
}

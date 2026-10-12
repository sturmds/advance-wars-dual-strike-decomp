#include "sdk/os_arm7.h"
#include "sdk/gx.h"
#include "sdk/pxi.h"
#include "sdk/ctrdg.h"
#include "sdk/spi.h"
#include "sdk/snd.h"

#define DISPSTAT_VBLANK_IRQ 0x8
#define IRQ_VBLANK 0x1

void func_arm7_027d5a24(int a0);
void UserSettings_LoadFromFlash(void);
s32 InitWramHeap(void);
BOOL func_arm7_037fe45c(void);
void FileSys_SetUpOnce_Arm7(u32 dmaNo);
void func_arm7_038028c8(int a0);
void Card_CheckRemovedEveryTenFrames(void);
void func_arm7_038058b0(int heap);
void VBlankIntr_Arm7(void);

/*
 * The ARM7 main loop; Entry_Arm7 jumps here. Runs the library setup calls,
 * loads the user settings from flash, makes the WRAM heap, installs and
 * enables the V-blank interrupt and turns on interrupts, then hands the heap
 * to func_arm7_038058b0. Each pass of the loop halts the CPU until the next
 * interrupt, then runs the per-frame work.
 */
void Arm7Main(void)
{
    int heap;

    System_StartServices_Arm7();
    Thread_SetUpTableOnce_Arm7();
    UserSettings_LoadFromFlash();
    Fifo_SetUp_Arm7();
    heap = InitWramHeap();
    Sound_StartDriver_Arm7(6);
    func_arm7_037fe45c();
    Irq_InstallHandlers_Arm7(IRQ_VBLANK, VBlankIntr_Arm7);
    Irq_SetEnableBits_Arm7(IRQ_VBLANK);
    /* needed to match: the original reads each register once before the
       write and ignores the value (the old IME setting) */
    *REG_DISPSTAT;
    *REG_DISPSTAT |= DISPSTAT_VBLANK_IRQ;
    *REG_IME;
    *REG_IME = 1;
    Cpu_UnmaskIrq_Arm7();
    FileSys_SetUpOnce_Arm7(-1);
    func_arm7_038028c8(15);
    func_arm7_027d5a24(12);
    func_arm7_038058b0(heap);
    SPI_Init(2);

    while (TRUE) {
        Cpu_IdleUntilIrq_Arm7();
        if (Reset_WasRequested_Arm7()) {
            Reset_QuiesceAndRestart_Arm7();
        }
        Cartridge_CheckRemovedEveryTenFrames();
        Card_CheckRemovedEveryTenFrames();
    }
}

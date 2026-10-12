#include "sdk/os.h"
#include "sdk/gx.h"

#define DISPSTAT_VBLANK_IRQ 0x8
#define IRQ_VBLANK 0x1

void func_arm7_027d5a24(int a0);
void UserSettings_LoadFromFlash(void);
int InitWramHeap(void);
void func_arm7_037f8444(void);                                  /* BIOS Halt (svc 6) */
OSIrqMask func_arm7_037fb690(OSIrqMask mask);                   /* enable IRQs in mask */
void func_arm7_037fb748(u32 mask, void (*handler)(void));       /* set IRQ handler */
void Thread_SetUpTableOnce_Arm7(void);
void func_arm7_037fc6c4(void);
int func_arm7_037fdb6c(void);
void func_arm7_037fdc24(void);
u16 func_arm7_037fdce4(void);
void func_arm7_037fe0a0(void);
BOOL func_arm7_037fe45c(void);
void Sound_StartDriver_Arm7(int a0);
void FileSys_SetUpOnce_Arm7(u32 dmaNo);
void func_arm7_038028c8(int a0);
void func_arm7_0380392c(void);
void SPI_Init(int a0);
void func_arm7_038058b0(int heap);
void Cartridge_CheckRemovedEveryTenFrames(void);
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

    func_arm7_037fc6c4();
    Thread_SetUpTableOnce_Arm7();
    UserSettings_LoadFromFlash();
    func_arm7_037fe0a0();
    heap = InitWramHeap();
    Sound_StartDriver_Arm7(6);
    func_arm7_037fe45c();
    func_arm7_037fb748(IRQ_VBLANK, VBlankIntr_Arm7);
    func_arm7_037fb690(IRQ_VBLANK);
    /* needed to match: the original reads each register once before the
       write and ignores the value (the old IME setting) */
    *REG_DISPSTAT;
    *REG_DISPSTAT |= DISPSTAT_VBLANK_IRQ;
    *REG_IME;
    *REG_IME = 1;
    func_arm7_037fdb6c();
    FileSys_SetUpOnce_Arm7(-1);
    func_arm7_038028c8(15);
    func_arm7_027d5a24(12);
    func_arm7_038058b0(heap);
    SPI_Init(2);

    while (TRUE) {
        func_arm7_037f8444();
        if (func_arm7_037fdce4()) {
            func_arm7_037fdc24();
        }
        Cartridge_CheckRemovedEveryTenFrames();
        func_arm7_0380392c();
    }
}

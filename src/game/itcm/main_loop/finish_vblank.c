#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;
extern u8 data_027c0000[];

void func_01ff9ebc(void);
void func_01ff9ed0(void);
void func_01ff9f1c(int value);
void func_01ff9f2c(void);

#define DTCM_IRQ_CHECK_FLAGS 0x3ff8   /* last 8 bytes of DTCM: interrupts the SDK waits for */
#define IRQ_CHECK_VBLANK 1

/* Runs vblankCallback if set, updates the frame flags in gMainLoop (the three
 * helpers are not named yet), then tells VBlankIntrWait that a VBlank happened. */
void MainLoop_FinishVBlank(void)
{
    u32 base;

    func_01ff9ebc();
    if (gMainLoop.vblankCallback != NULL) {
        gMainLoop.vblankCallback();
    }
    func_01ff9f2c();
    func_01ff9ed0();
    func_01ff9f1c(0);
    /* needed to match: the address is built in an integer, so the compiler adds the 0x3000 at run time */
    base = (u32)data_027c0000 + 0x3000;
    *(u32 *)(base + (DTCM_IRQ_CHECK_FLAGS - 0x3000)) |= IRQ_CHECK_VBLANK;
}

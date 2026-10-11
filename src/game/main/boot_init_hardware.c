#include "types.h"
#include "sdk/gx.h"

void func_0210e188(void);
void func_0210e25c(int a, int b);
void func_0210e270(int a, int b);
void func_0210e67c(int a);
void func_0210a420(void);
void func_0210a5fc(void);
void func_0210ce40(int a, void (*callback)(void));
void func_0210cd3c(int a);
void GX_VBlankIntr(void);
void MainLoop_BootVBlank(void);

#define REG_IME ((volatile u16 *)0x04000208)
#define POWCNT1_ENGINES 0x20e /* 2D engine A, 3D rendering, 3D geometry, 2D engine B */

/* First step of boot: sets up memory regions, switches the display and 3D
 * engines off and on around func_0210a5fc, installs MainLoop_BootVBlank as
 * the handler for interrupt mask 1 (VBlank), then enables interrupts. The unused read of
 * IME is needed to match. */
void Boot_InitHardware(void)
{
    u16 oldIme;

    func_0210e188();
    func_0210e25c(2, 0x2400000);
    func_0210e270(2, 0x2700000);
    func_0210e67c(0x200002d);
    func_0210a420();
    *REG_POWCNT1 = *REG_POWCNT1 & ~POWCNT1_ENGINES;
    func_0210a5fc();
    *REG_POWCNT1 = (*REG_POWCNT1 & ~POWCNT1_ENGINES) | POWCNT1_ENGINES;
    func_0210ce40(1, MainLoop_BootVBlank);
    func_0210cd3c(1);
    func_0210cd3c(0x40000);
    oldIme = *REG_IME;
    *REG_IME = 1;
    GX_VBlankIntr();
}

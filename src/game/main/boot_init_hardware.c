#include "types.h"
#include "sdk/gx.h"

void OS_Init(void);
void OS_SetArenaLo(int a, int b);
void OS_SetArenaHi(int a, int b);
void OS_SetProtectionRegion1(int a);
void func_0210a420(void);
void OS_SetIrqFunction(int a, void (*callback)(void));
void OS_EnableIrqMask(int a);
void GX_VBlankIntr(void);
void MainLoop_BootVBlank(void);

#define REG_IME ((volatile u16*)0x04000208)
#define POWCNT1_ENGINES \
  0x20e /* 2D engine A, 3D rendering, 3D geometry, 2D engine B */

/* First step of boot: sets up memory regions, switches the display and 3D
 * engines off and on around GX_Init, installs MainLoop_BootVBlank as
 * the handler for interrupt mask 1 (VBlank), then enables interrupts. The unused read of
 * IME is needed to match. */
void Boot_InitHardware(void) {
  u16 oldIme;

  OS_Init();
  OS_SetArenaLo(2, 0x2400000);
  OS_SetArenaHi(2, 0x2700000);
  OS_SetProtectionRegion1(0x200002d);
  func_0210a420();
  *REG_POWCNT1 = *REG_POWCNT1 & ~POWCNT1_ENGINES;
  GX_Init();
  *REG_POWCNT1 = (*REG_POWCNT1 & ~POWCNT1_ENGINES) | POWCNT1_ENGINES;
  OS_SetIrqFunction(1, MainLoop_BootVBlank);
  OS_EnableIrqMask(1);
  OS_EnableIrqMask(0x40000);
  oldIme = *REG_IME;
  *REG_IME = 1;
  GX_VBlankIntr();
}

#include "types.h"
#include "sdk/gx.h"

void Boot_InitHardware(void);
void Gfx_ClearVideoMemory(void);
void Vram_AssignAllBanks(void);
void SVC_WaitVBlankIntr(void);

#define REG_EXMEMCNT ((volatile u16 *)0x04000204)
#define EXMEMCNT_BIT15 0x8000       /* guess: main memory interface priority */
#define DISPCNT_MODE_GRAPHICS 0x10000   /* display mode 1: show the engine's own picture */
#define MASTER_BRIGHT_WHITE 16

/* Boot: runs the hardware, video memory and VRAM bank setup, waits for a
 * VBlank, fills both screens with white and turns the displays on. */
void Boot_EnableDisplay(void)
{
    *REG_EXMEMCNT |= EXMEMCNT_BIT15;
    Boot_InitHardware();
    Gfx_ClearVideoMemory();
    Vram_AssignAllBanks();
    SVC_WaitVBlankIntr();
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, MASTER_BRIGHT_WHITE);
    GXx_SetMasterBrightness_(IO_SUB_SCREEN_FADE, MASTER_BRIGHT_WHITE);
    GX_DispOn();
    *REG_DB_DISPCNT |= DISPCNT_MODE_GRAPHICS;
}

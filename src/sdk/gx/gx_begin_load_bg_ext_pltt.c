#include "types.h"
#include "sdk/gx.h"
#include "sdk/gx_load.h"

/* Unmaps the main BG extended palette banks and returns them (best guess:
 * GX_ResetBankForBGExtPltt). */
int func_0210a95c(void);

/* Bank E holds all four palette slots. G alone holds slots 2-3, so a load
 * at palette offset 0x4000 starts at G's first byte. F, or F with G, holds
 * slots 0-1 first. */
void GX_BeginLoadBGExtPltt(void)
{
    data_022a98a0 = func_0210a95c();
    switch (data_022a98a0) {
    case 0:
        break;
    case GX_VRAM_E:
        data_022a989c = HW_LCDC_VRAM_E;
        data_022a9898 = 0;
        break;
    case GX_VRAM_G:
        data_022a989c = HW_LCDC_VRAM_G;
        data_022a9898 = 0x4000;
        break;
    case GX_VRAM_F:
    case GX_VRAM_F | GX_VRAM_G:
        data_022a989c = HW_LCDC_VRAM_F;
        data_022a9898 = 0;
        break;
    }
}

#include "types.h"
#include "sdk/gx.h"

extern u16 data_022a9878;
void func_0210b470(u32 banks);

/* Moves the banks in *state back to the LCDC mask and switches them on again. Returns the old mask. */
u32 Vram_ReturnBanksToLcdc(u16 *state)
{
    u32 mask = *state;

    *state = 0;
    data_022a9878 |= mask;
    func_0210b470(mask);
    return mask;
}

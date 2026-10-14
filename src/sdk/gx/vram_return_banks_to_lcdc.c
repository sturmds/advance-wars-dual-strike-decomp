#include "types.h"
#include "sdk/gx.h"

/* Moves the banks in *state back to the LCDC mask and switches them on again. Returns the old mask. */
u32 Vram_ReturnBanksToLcdc(u16* state) {
  u32 mask = *state;

  *state = 0;
  gGxVramBankMasks[0] |= mask;
  Vram_PowerOnBanks(mask);
  return mask;
}

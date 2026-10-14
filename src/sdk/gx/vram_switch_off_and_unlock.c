#include "types.h"
#include "sdk/gx.h"

void OSi_UnlockVram(u16 banks, u16 lockId);

/* Clears the VRAMCNT of every bank whose bit (A = bit 0 ... I = bit 8) is set in *banks. Returns the old mask. */
u32 Vram_SwitchOffAndUnlock(u16* banks) {
  u32 mask = *banks;

  *banks = 0;
  if (mask & 0x001) {
    *REG_VRAMCNT_A = 0;
  }
  if (mask & 0x002) {
    *REG_VRAMCNT_B = 0;
  }
  if (mask & 0x004) {
    *REG_VRAMCNT_C = 0;
  }
  if (mask & 0x008) {
    *REG_VRAMCNT_D = 0;
  }
  if (mask & 0x010) {
    *REG_VRAMCNT_E = 0;
  }
  if (mask & 0x020) {
    *REG_VRAMCNT_F = 0;
  }
  if (mask & 0x040) {
    *REG_VRAMCNT_G = 0;
  }
  if (mask & 0x080) {
    *REG_VRAMCNT_H = 0;
  }
  if (mask & 0x100) {
    *REG_VRAMCNT_I = 0;
  }
  OSi_UnlockVram(mask, GXi_VRamLockId);
  return mask;
}

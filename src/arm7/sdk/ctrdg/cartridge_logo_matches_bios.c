#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

/*
 * TRUE when the logo in the GBA cartridge header equals the copy of the
 * ARM9 BIOS logo that the ARM9 sent.
 */
BOOL Cartridge_LogoMatchesBios(const u16* logo) {
  GbaRomHeader* header = GBA_SLOT_HEADER_PTR;
  const u16* romLogo = header->logo;
  u16 lockID;
  BOOL same = TRUE;
  int i;

  lockID = LockId_Allocate_Arm7();
  SlotLock_Acquire_Arm7(lockID);
  for (i = 0; i < GBA_LOGO_LEN_HALFWORDS; i++) {
    /* Header bytes 0x9c and 0x9e hold flag bits inside the logo; they are not compared. */
    u16 mask = 0xffff;
    if (i == 76) {
      mask ^= 0x84;
    } else if (i == 77) {
      mask ^= 3;
    }
    if ((logo[i] & mask) != romLogo[i]) {
      same = FALSE;
      break;
    }
  }
  SlotLock_ReleaseAlias_Arm7(lockID);
  LockId_Free_Arm7(lockID);
  return same;
}

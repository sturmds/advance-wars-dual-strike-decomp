#include "types.h"
#include "sdk/gx.h"
#include "sdk/os.h"

u16 gGxDispMode;
/* also declared in src/sdk/gx/vram_switch_off_and_unlock.c */
u16 GXi_VRamLockId;
extern u32
    data_0216d958; /* DMA channel the GX library uses, or GX_DMA_NOT_USE */

void MI_DmaFill32(u32 dmaNo, void* dest, u32 data, u32 size); /* MI_DmaFill32 */
void MIi_CpuClear32(u32 data, void* dest, u32 size); /* MIi_CpuClear32 */

static inline void CpuFill32(void* dest, u32 data, u32 size) {
  volatile u32 fill =
      data; /* needed to match: the original passes data through the stack */

  MIi_CpuClear32(fill, dest, size);
}

/* Powers up both 2D engines and the LCDs, takes a lock id for VRAM, and
 * clears the display registers of both engines. */
void GX_Init(void) {
  *REG_POWCNT1 |= POWCNT1_DISPLAY_SWAP;
  *REG_POWCNT1 = (*REG_POWCNT1 & ~POWCNT1_ENGINES) | POWCNT1_ENGINES;
  *REG_POWCNT1 |= POWCNT1_LCD;
  Vram_ClearAllBankControl();

  while (GXi_VRamLockId == 0) {
    int lockId = OS_GetLockID();
    if (lockId == OS_LOCK_ID_ERROR) {
      OS_Terminate();
    }
    GXi_VRamLockId = lockId;
  }

  *REG_DISPSTAT = 0;
  *REG_DISPCNT = 0;
  if (data_0216d958 != GX_DMA_NOT_USE) {
    /* Main engine: BG0CNT up to 0x04000068. Sub engine: DISPCNT through MASTER_BRIGHT. */
    MI_DmaFill32(data_0216d958, (void*)REG_BG0CNT, 0, 0x60);
    *REG_MASTER_BRIGHT = 0;
    MI_DmaFill32(data_0216d958, (void*)REG_DB_DISPCNT, 0, 0x70);
  } else {
    CpuFill32((void*)REG_BG0CNT, 0, 0x60);
    *REG_MASTER_BRIGHT = 0;
    CpuFill32((void*)REG_DB_DISPCNT, 0, 0x70);
  }

  *REG_BG2PA = 0x100;
  *REG_BG2PD = 0x100;
  *REG_BG3PA = 0x100;
  *REG_BG3PD = 0x100;
  *REG_DB_BG2PA = 0x100;
  *REG_DB_BG2PD = 0x100;
  *REG_DB_BG3PA = 0x100;
  *REG_DB_BG3PD = 0x100;
}

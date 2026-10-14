#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"
#include "sdk/gx_load.h"

void MI_DmaCopy16(u32 dmaNo, u32 src, u32 dest, u32 size);
void DC_FlushAll(void);

/* POSTFLG: bit 0 is set once the ARM7 and ARM9 finished their boot. */
#define REG_POSTFLG (*(vu16*)0x04000300)

/* EXMEMCNT bit 15: set when the ARM7 owns the GBA slot, clear for the ARM9. */
#define EXMEMCNT_SLOT2_ARM7 0x8000

/* The logo the GBA checks sits in the ARM9 BIOS; the ARM7 compares it with the copy sent to it. */
#define BIOS_AGB_LOGO ((const void*)0xffff0020)
#define AGB_LOGO_SIZE 0x9c

/* The second half of the header, 0x80 to 0xbf, is read from the cartridge. */
#define AGB_HEADER_TAIL_OFFSET 0x80
#define AGB_HEADER_TAIL_SIZE 0x40

static u32 gCtrdgHeaderRead;
extern GbaRomHeader data_022acf40;
extern u8 data_022acf44[];

/* Needed to match: the original was built with these two optimizations off. */
#pragma opt_common_subs off
#pragma opt_strength_reduction off

static inline void ClearOwner(vu16* reg) { *reg = *reg & ~EXMEMCNT_SLOT2_ARM7; }

static inline void SetOwner(vu16* reg, u32 owner) {
  *reg = (*reg & ~EXMEMCNT_SLOT2_ARM7) | (owner << 15);
}

void Cartridge_CopyGbaHeaderOnce(void) {
  vu16* exmemcnt;
  int i;
  GbaSlotLockRecord lock;
  GbaSlotBootSnapshot* info;
  OSIrqMask oldIrqMask;
  GbaSlotWaitStates timing;
  u32 oldSlotOwner;
  u16 oldIme;

  if (gCtrdgHeaderRead != 0) {
    return;
  }
  gCtrdgHeaderRead = 1;
  if ((REG_POSTFLG & 1) == 0) {
    return;
  }
  oldIrqMask = OS_SetIrqMask(IRQ_FIFO_RECV);
  oldIme = *REG_IME;
  *REG_IME = 1;
  Cartridge_SpinUntilSlotOwned(data_022acf14.lockID, &lock);
  oldSlotOwner = (REG_EXMEMCNT & EXMEMCNT_SLOT2_ARM7) >> 15;
  Cartridge_StashAndSlowRomWaits(&timing);
  exmemcnt = &REG_EXMEMCNT;
  ClearOwner(exmemcnt);
  MI_DmaCopy16(1, (u32)GBA_SLOT_HEADER_PTR + AGB_HEADER_TAIL_OFFSET,
               (u32)&data_022acf40 + AGB_HEADER_TAIL_OFFSET,
               AGB_HEADER_TAIL_SIZE);
  SetOwner(exmemcnt, oldSlotOwner);
  Cartridge_RestoreRomWaits(&timing);
  Cartridge_ReleaseSlotIfTaken(data_022acf14.lockID, &lock);

  info = GBA_SLOT_SNAPSHOT_PTR;
  info->reserved_be = data_022acf40.reserved_be;
  for (i = 0; i < (int)ARRAY_COUNT(info->reserved_b5); i++) {
    info->reserved_b5[i] = data_022acf40.reserved_b5[i];
  }
  info->makerCode = data_022acf40.makerCode;
  info->gameCode = data_022acf40.gameCode;

  MIi_CpuCopy32(BIOS_AGB_LOGO, data_022acf44, AGB_LOGO_SIZE);
  DC_FlushAll();
  Cartridge_SendWordUntilTaken((((u32)&data_022acf40 - 0x02000000) >> 5) << 6 |
                               GBA_SLOT_MSG_SETUP_ACK);
  while (data_022acf14.commandDone != 1) {
    SVC_WaitByLoop(1);
  }
  (void)*REG_IME;
  *REG_IME = oldIme;
  OS_SetIrqMask(oldIrqMask);
}

#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

void func_arm7_037fdc04(u32 count);             /* SVC_WaitByLoop through a veneer */
void func_arm7_038064b0(u32 word);              /* Cartridge_SendWordUntilTaken */
BOOL Cartridge_LogoMatchesBios(const u16 *logo);

/* POSTFLG: bit 0 is set once the ARM7 and ARM9 finished their boot. */
#define REG_POSTFLG (*(vu16 *)0x04000300)

/*
 * The init word from the ARM9 holds the address of its copy of the cartridge
 * header above the command bits, as an offset into main RAM in 32-byte units.
 */
#define MAIN_RAM_BASE 0x02000000
#define INIT_WORD_OFFSET_MASK 0x1ffffc0
#define INIT_WORD_OFFSET_SHIFT 6
#define INIT_WORD_OFFSET_UNIT_SHIFT 5

extern u32 gCtrdgHeaderReceived;
extern GbaSlotInitWord gCtrdgInitMessage;

/*
 * Runs once after boot: waits for the ARM9's init word, compares the BIOS
 * logo the ARM9 put in its header buffer with the cartridge's logo, records
 * the result as isAgb and answers the ARM9.
 */
void Cartridge_CheckLogoFromArm9Once(void)
{
    OSIrqMask oldIrqMask;
    u16 oldIme;
    GbaRomHeader *header;
    GbaSlotBootSnapshot *info;
    BOOL logoOk;

    if (gCtrdgHeaderReceived != 0) {
        return;
    }
    gCtrdgHeaderReceived = 1;
    if ((REG_POSTFLG & 1) == 0) {
        return;
    }
    oldIrqMask = Irq_ReplaceEnableBits_Arm7(IRQ_FIFO_RECV);
    oldIme = *REG_IME;
    *REG_IME = 1;
    while (gCtrdgInitMessage.received != 1) {
        func_arm7_037fdc04(0x100);
    }
    header = (GbaRomHeader *)((((gCtrdgInitMessage.word & INIT_WORD_OFFSET_MASK) >> INIT_WORD_OFFSET_SHIFT)
                                 << INIT_WORD_OFFSET_UNIT_SHIFT) + MAIN_RAM_BASE);
    logoOk = Cartridge_LogoMatchesBios(header->logo);
    info = GBA_SLOT_SNAPSHOT_PTR;
    info->isAgb = logoOk;
    func_arm7_038064b0(GBA_SLOT_MSG_SETUP_ACK);
    (void)*REG_IME;
    *REG_IME = oldIme;
    Irq_ReplaceEnableBits_Arm7(oldIrqMask);
}

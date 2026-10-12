#include "types.h"
#include "sdk/rom_header.h"
#include "sdk/os_arm7.h"

extern u32 data_arm7_03809248;             /* DMA channel for cartridge reads */
extern s32 data_arm7_0380924c;             /* lock id for the cartridge bus */
/* Overlay tables already in memory: offset is their address, 0 if none. */
extern RomRegion data_arm7_03809250;       /* ARM9 */
extern RomRegion data_arm7_03809258;       /* ARM7 */

void Card_StartService_Arm7(void);

/* The ARM7 half of FSi_InitRom: it keeps the DMA channel, takes a lock id and
 * starts the card library, but sets up no archive. */
void FileSys_StartCardAccess_Arm7(u32 dmaNo)
{
    data_arm7_03809248 = dmaNo;
    data_arm7_0380924c = LockId_Allocate_Arm7();
    data_arm7_03809250.offset = 0;
    data_arm7_03809250.length = 0;
    data_arm7_03809258.offset = 0;
    data_arm7_03809258.length = 0;
    Card_StartService_Arm7();
}

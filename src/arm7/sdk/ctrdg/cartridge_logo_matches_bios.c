#include "types.h"
#include "sdk/ctrdg.h"

int func_arm7_037fb81c(void);              /* OS_GetLockID */
void func_arm7_037fb944(int lockID);       /* locks the cartridge bus, waiting for it */
void func_arm7_037fb810(int lockID);       /* OS_UnlockCartridge */
void func_arm7_037fb8b4(int lockID);       /* OS_ReleaseLockID */

/*
 * TRUE when the logo in the GBA cartridge header equals the copy of the
 * ARM9 BIOS logo that the ARM9 sent.
 */
BOOL Cartridge_LogoMatchesBios(const u16 *logo)
{
    GbaRomHeader *header = GBA_SLOT_HEADER_PTR;
    const u16 *romLogo = header->logo;
    u32 lockIDCopy;
    BOOL same = TRUE;
    u16 lockID = func_arm7_037fb81c();
    int i;

    func_arm7_037fb944(lockID);
    lockIDCopy = lockID;   /* needed to match: the original keeps a second copy */
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
    func_arm7_037fb810(lockIDCopy);
    func_arm7_037fb8b4(lockIDCopy);
    return same;
}

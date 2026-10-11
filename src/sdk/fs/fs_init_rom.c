/* Needed to match: the original keeps the two ROM header pointers in
 * registers across the call instead of folding them into each load. */
#pragma opt_propagation off

#include "sdk/fs.h"
#include "sdk/os.h"
#include "sdk/rom_header.h"

extern u32 data_022ab840;              /* DMA channel for cartridge reads */
extern u32 data_022ab83c;              /* lock id for the cartridge bus */
/* Overlay tables already in memory: offset is their address, 0 if none. */
extern RomRegion data_022ab844;        /* ARM9 */
extern RomRegion data_022ab84c;        /* ARM7 */
extern const char data_0216d974[];     /* "rom" */

void FS_InitArchive(FSArchive *arc);
BOOL FS_RegisterArchiveName(FSArchive *arc, const char *name, int nameLength);

s32 OS_GetLockID(void);
void func_02115fe8(void);              /* looks like CARD_Init */
/* Sets the archive's command routine and the commands it handles
 * (FS_SetArchiveProc in the SDK; the current name is older). */
void Archive_AssignTableCache(FSArchive *arc, ArchiveCommandHook proc, u32 commands);
BOOL FS_LoadArchive(FSArchive *arc, u32 base, u32 fat, u32 fatSize, u32 fnt, u32 fntSize,
                    ArchiveByteReader read, ArchiveByteWriter write);

int FSi_EmptyArchiveProc(FSFile *file, u32 command);
int FSi_ReadDummyCallback(FSArchive *arc, void *dest, u32 src, u32 len);
int FSi_RomArchiveProc(FSFile *file, u32 command);
int FSi_WriteDummyCallback(FSArchive *arc, const void *src, u32 dest, u32 len);
int FSi_ReadRomCallback(FSArchive *arc, void *dest, u32 src, u32 len);

/* Sets up the "rom" archive over the cartridge file system. A program
 * started by Download Play has no cartridge files: its archive answers
 * every command as unsupported and both overlay tables are marked missing.
 * A cartridge header without a FAT or FNT leaves the archive unloaded. */
void FSi_InitRom(u32 dmaNo)
{
    data_022ab840 = dmaNo;
    data_022ab83c = OS_GetLockID();
    data_022ab844.offset = 0;
    data_022ab844.length = 0;
    data_022ab84c.offset = 0;
    data_022ab84c.length = 0;
    func_02115fe8();
    FS_InitArchive(&gRomFileArchive);
    FS_RegisterArchiveName(&gRomFileArchive, data_0216d974, 3);

    if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
        data_022ab844.offset = 0xffffffff;
        data_022ab844.length = 0;
        data_022ab84c.offset = 0xffffffff;
        data_022ab84c.length = 0;
        Archive_AssignTableCache(&gRomFileArchive, FSi_EmptyArchiveProc, 0xffffffff);
        FS_LoadArchive(&gRomFileArchive, 0, 0, 0, 0, 0, FSi_ReadDummyCallback, FSi_WriteDummyCallback);
    } else {
        const RomRegion *fnt = &ROM_HEADER_BUF->fnt;
        const RomRegion *fat = &ROM_HEADER_BUF->fat;

        Archive_AssignTableCache(&gRomFileArchive, FSi_RomArchiveProc,
                           (1 << ARCHIVE_OP_COPY_IN) | (1 << FS_COMMAND_ACTIVATE) | (1 << FS_COMMAND_IDLE));
        if (fnt->offset == 0xffffffff || fnt->offset == 0 || fat->offset == 0xffffffff || fat->offset == 0) {
            return;
        }
        FS_LoadArchive(&gRomFileArchive, 0, fat->offset, fat->length, fnt->offset, fnt->length,
                       FSi_ReadRomCallback, FSi_WriteDummyCallback);
    }
}

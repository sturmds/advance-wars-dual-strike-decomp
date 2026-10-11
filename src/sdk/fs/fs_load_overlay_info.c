#include "sdk/fs.h"
#include "sdk/rom_header.h"

extern RomRegion data_022ab844;
extern RomRegion data_022ab84c;

void Mem_CopyBytes(const void *src, void *dst, u32 size);
BOOL FS_OpenFileFast(FSFile *file, FSFileID id);
BOOL FSi_LoadOverlayInfoCore(FSOverlayInfo *info, MIProcessor target, u32 id, FSArchive *arc,
                   RomRegion arm9Overlays, RomRegion arm7Overlays);

BOOL func_021133a0(FSOverlayInfo *info, MIProcessor target, u32 id)
{
    RomRegion *table = target == MI_PROCESSOR_ARM9 ? &data_022ab844 : &data_022ab84c;

    if (table->offset != 0) {
        u32 pos = id * sizeof(OverlayTableRow);
        if (pos >= table->length) {
            return FALSE;
        }
        Mem_CopyBytes((const void *)(table->offset + pos), info, sizeof(OverlayTableRow));
        info->target = target;
        {
            FSFile file;
            FS_InitFile(&file);
            if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(info))) {
                return FALSE;
            }
            info->romTop = file.top;
            info->romSize = file.bottom - file.top;
            FS_CloseFile(&file);
        }
        return TRUE;
    }
    return FSi_LoadOverlayInfoCore(info, target, id, &gRomFileArchive, ROM_HEADER_BUF->arm9Overlays,
                         ROM_HEADER_BUF->arm7Overlays);
}

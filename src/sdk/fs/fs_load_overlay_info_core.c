#include "sdk/fs.h"

BOOL FS_OpenFileFast(FSFile *file, FSFileID id);

/* Reads entry id of the target CPU's overlay table from the archive into
 * info, then looks up where the overlay's file lies. The two tables come as
 * four words, offset and length of each. */
BOOL FSi_LoadOverlayInfoCore(FSOverlayInfo *info, MIProcessor target, u32 id, FSArchive *arc,
                   u32 arm9Offset, u32 arm9Length, u32 arm7Offset, u32 arm7Length)
{
    FSFile file;
    u32 pos;
    u32 offset;
    u32 length;

    if (target == MI_PROCESSOR_ARM9) {
        length = arm9Length;
        offset = arm9Offset;
    } else {
        length = arm7Length;
        offset = arm7Offset;
    }
    pos = id * sizeof(OverlayTableRow);
    if (pos >= length) {
        return FALSE;
    }
    FS_InitFile(&file);
    if (!FS_OpenFileDirect(&file, arc, offset + pos, offset + length, 0xffffffff)) {
        return FALSE;
    }
    if (FS_ReadFile(&file, info, sizeof(OverlayTableRow)) != sizeof(OverlayTableRow)) {
        FS_CloseFile(&file);
        return FALSE;
    }
    FS_CloseFile(&file);
    info->target = target;
    if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(info))) {
        return FALSE;
    }
    info->romTop = file.top;
    info->romSize = file.bottom - file.top;
    FS_CloseFile(&file);
    return TRUE;
}

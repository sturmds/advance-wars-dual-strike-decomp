#include "sdk/fs.h"

BOOL func_021128b4(FSFile *file, FSFileID id);
u32 func_02113630(const FSOverlayInfo *info);
void func_021135ec(const FSOverlayInfo *info);

/* Reads the overlay file into the overlay's memory. */
BOOL FS_LoadOverlayImage(const FSOverlayInfo *info)
{
    FSFileID id;
    FSFile file;
    u32 size;

    FS_InitFile(&file);
    id = FS_GetOverlayFileID(info);
    if (!func_021128b4(&file, id)) {
        return FALSE;
    }
    size = func_02113630(info);
    func_021135ec(info);
    if (FS_ReadFile(&file, info->header.ram_address, size) != size) {
        FS_CloseFile(&file);
        return FALSE;
    }
    FS_CloseFile(&file);
    return TRUE;
}

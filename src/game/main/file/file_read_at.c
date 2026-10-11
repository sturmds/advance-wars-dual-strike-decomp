#include "types.h"
#include "sdk/fs.h"
#include "sdk/gx.h"
#include "sdk/os.h"

/* Needed to match: the original builds this test as a 0/1 value first, as an
 * inline function does. */
static inline BOOL IsFile(FSFile *file)
{
    if (file->flags & FS_FILE_STATUS_IS_FILE) {
        return TRUE;
    }
    return FALSE;
}

/* Reads size bytes starting offset bytes into the file at path, to dest, and
 * returns size. If the file cannot be opened or read, turns the main screen
 * bright and halts (like File_Load). */
u32 File_ReadAt(void *dest, const char *path, s32 offset, u32 size)
{
    FSFile file;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path)) {
        if (IsFile(&file) == FALSE) {
            OS_Terminate();
        }
        if (!FS_SeekFile(&file, offset, 0)) {
            GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
            OS_Terminate();
            FS_CloseFile(&file);
            return 0;
        }
        if (size != FS_ReadFile(&file, dest, size)) {
            GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
            OS_Terminate();
            FS_CloseFile(&file);
            return 0;
        }
        FS_CloseFile(&file);
    } else {
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
        OS_Terminate();
        return 0;
    }
    return size;
}

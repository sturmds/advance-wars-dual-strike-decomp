#include "sdk/fs.h"

/* Command routine of an archive with no files: handles no command. */
int FSi_EmptyArchiveProc(FSFile *file, u32 command)
{
    return FS_RESULT_UNSUPPORTED;
}

#include "sdk/fs.h"

int func_02111bc4(FSFile *file, u32 command);   /* runs one command through the table */
FSFile *func_0211232c(FSArchive *arc);        /* picks the next command to run */
void func_02112290(FSFile *file);

/* Runs the file's command, releases it with the result, then starts the
 * next waiting command of the archive. Returns TRUE when the command
 * succeeded. */
BOOL FSi_ExecuteSyncCommand(FSFile *file)
{
    FSFile *next;

    FSi_ReleaseCommand(file, func_02111bc4(file, file->command));
    next = func_0211232c(file->arc);
    if (next != NULL) {
        func_02112290(next);
    }
    return file->error == FS_RESULT_SUCCESS;
}

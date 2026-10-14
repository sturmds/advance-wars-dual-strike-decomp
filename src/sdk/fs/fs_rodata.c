#include "types.h"

void FSi_ReadFileCommand(void);
void FSi_WriteFileCommand(void);
void Archive_CmdSetDirPosition(void);
void Archive_CmdNextDirEntry(void);
void FSi_FindPathCommand(void);
void FSi_GetPathCommand(void);
void FSi_OpenFileFastCommand(void);
void FSi_OpenFileDirectCommand(void);
void func_021111dc(void);

/* declared `int (*const gArchiveFallbackCommands[])(FSFile* file)` in fs_translate_command.c: cannot read the declaration `int (*const gArchiveFallbackCommands[])(FSFile* file)` */
void* const gArchiveFallbackCommands[9] = {&FSi_ReadFileCommand,
                                           &FSi_WriteFileCommand,
                                           &Archive_CmdSetDirPosition,
                                           &Archive_CmdNextDirEntry,
                                           &FSi_FindPathCommand,
                                           &FSi_GetPathCommand,
                                           &FSi_OpenFileFastCommand,
                                           &FSi_OpenFileDirectCommand,
                                           &func_021111dc};
const u32 data_0215192c[16] = {
#include "data/main/data_0215192c.inc.c"
};

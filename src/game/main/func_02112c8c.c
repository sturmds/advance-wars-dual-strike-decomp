#include "types.h"

void FS_LoadArchiveTables(void *, int, int, int);
extern u8 gRomFileArchive[];

void func_02112c8c(int a0, int a1)
{
    FS_LoadArchiveTables(gRomFileArchive, a0, a1, a0);
}

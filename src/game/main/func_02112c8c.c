#include "types.h"
#include "sdk/fs.h"

void FS_LoadArchiveTables(void*, int, int, int);

void func_02112c8c(int a0, int a1) {
  FS_LoadArchiveTables(&gRomFileArchive, a0, a1, a0);
}

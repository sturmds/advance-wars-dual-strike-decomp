#include "types.h"
#include "sdk/fs.h"

u32 Ov004_GetFileSize(const char* path) {
  FSFile file;
  u32 size;

  FS_InitFile(&file);
  FS_OpenFile(&file, path);
  size = file.bottom - file.top;
  FS_CloseFile(&file);
  return size;
}

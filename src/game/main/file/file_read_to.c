#include "types.h"
#include "sdk/fs.h"
#include "sdk/gx.h"
#include "sdk/os.h"

/* Reads a whole file into dest and returns its size. If the file cannot be
 * opened or read, turns the main screen bright and halts (like File_Load). */
u32 File_ReadTo(void* dest, const char* path) {
  FSFile file;
  u32 size;

  FS_InitFile(&file);
  if (FS_OpenFile(&file, path)) {
    size = file.bottom - file.top;
    if (size != FS_ReadFile(&file, dest, size)) {
      FS_CloseFile(&file);
      GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
      OS_Terminate();
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

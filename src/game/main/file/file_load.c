#include "types.h"
#include "sdk/fs.h"
#include "sdk/gx.h"
#include "sdk/os.h"

void* Mem_Alloc(s32 size); /* allocates from the game heap */
void Mem_Free(void* block);

/* Reads a whole file into a new heap block and returns it. If the file
 * cannot be opened or read, turns the main screen bright and halts. */
void* File_Load(const char* path) {
  FSFile file;
  s32 size;
  void* data;

  FS_InitFile(&file);
  if (FS_OpenFile(&file, path)) {
    size = file.bottom - file.top;
    data = Mem_Alloc(size);
    if (size != FS_ReadFile(&file, data, size)) {
      FS_CloseFile(&file);
      Mem_Free(data);
      GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
      OS_Terminate();
      return NULL;
    }
    FS_CloseFile(&file);
  } else {
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
    OS_Terminate();
    return NULL;
  }
  return data;
}

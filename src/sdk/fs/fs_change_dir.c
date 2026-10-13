#include "sdk/fs.h"

BOOL FS_ChangeDir(const char* path) {
  FSDirPos dir;
  FSFile file;

  FS_InitFile(&file);
  if (!FSi_FindPath(&file, path, NULL, &dir)) {
    return FALSE;
  }
  data_022ab82c = dir;
  return TRUE;
}

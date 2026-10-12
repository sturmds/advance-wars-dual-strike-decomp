#include "sdk/fs.h"

BOOL FS_ConvertPathToFileID(FSFileID* fileId, const char* path) {
  FSFile file;

  FS_InitFile(&file);
  return FSi_FindPath(&file, path, fileId, NULL) != 0;
}

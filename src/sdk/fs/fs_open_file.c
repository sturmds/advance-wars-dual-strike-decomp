#include "sdk/fs.h"

BOOL FS_ConvertPathToFileID(FSFileID* fileId,
                            const char* path);       /* path to file ID */
BOOL FS_OpenFileFast(FSFile* file, FSFileID fileId); /* opens by file ID */

BOOL FS_OpenFile(FSFile* file, const char* path) {
  FSFileID fileId;

  if (FS_ConvertPathToFileID(&fileId, path)) {
    if (FS_OpenFileFast(file, fileId)) {
      return TRUE;
    }
  }
  return FALSE;
}

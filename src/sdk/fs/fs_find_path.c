#include "sdk/fs.h"

extern FSDirPos data_022ab82c; /* current directory */

static inline BOOL FSi_IsSlash(u32 c) { return c == '/' || c == '\\'; }

static inline BOOL Archive_RoutinesSet(FSArchive* arc) {
  return (arc->flags & ARCHIVE_STATE_ROUTINES_SET) ? TRUE : FALSE;
}

/* Sends the find-path command for path on dir. The path starts at the
 * current directory, at the root of the current archive after a leading
 * slash, or at the root of a named archive ("rom:/..."). With dirPos set
 * it looks for a directory and writes its position there; otherwise it
 * looks for a file and writes its id to fileId. Fails on an unknown or
 * unloaded archive. */
BOOL FSi_FindPath(FSFile* dir, const char* path, FSFileID* fileId,
                  FSDirPos* dirPos) {
  FSDirPos start;

  if (FSi_IsSlash((u8)*path)) {
    start.arc = data_022ab82c.arc;
    start.ownId = 0;
    start.pos = 0;
    start.index = 0;
    path++;
  } else {
    int i;

    start = data_022ab82c;
    for (i = 0; i <= ARCHIVE_NAME_MAX_CHARS; i++) {
      u32 c = (u8)path[i];

      if (c == '\0' || FSi_IsSlash(c)) {
        break;
      } else if (c == ':') {
        FSArchive* arc = FS_FindArchive(path, i);

        if (arc == NULL) {
          return FALSE;
        }
        if (Archive_RoutinesSet(arc) == FALSE) {
          return FALSE;
        }
        start.arc = arc;
        start.pos = 0;
        start.index = 0;
        start.ownId = 0;
        path += i + 1;
        if (FSi_IsSlash((u8)*path)) {
          path++;
        }
        break;
      }
    }
  }
  dir->arc = start.arc;
  dir->arg.findPath.path = path;
  dir->arg.findPath.pos = start;
  if (dirPos != NULL) {
    dir->arg.findPath.findDirectory = TRUE;
    dir->arg.findPath.result.dir = dirPos;
  } else {
    dir->arg.findPath.findDirectory = FALSE;
    dir->arg.findPath.result.file = fileId;
  }
  return FSi_SendCommand(dir, ARCHIVE_OP_RESOLVE_PATH);
}

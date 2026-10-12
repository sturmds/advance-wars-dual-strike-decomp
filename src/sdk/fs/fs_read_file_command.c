#include "sdk/fs.h"

/* Archive command: reads the requested bytes from the file position through
 * the archive's read routine and moves the position past them. */
int FSi_ReadFileCommand(FSFile* file) {
  u32 pos = file->pos;
  u32 length = file->arg.readFile.length;
  FSArchive* arc = file->arc;
  void* dest = file->arg.readFile.dest;

  file->pos = pos + length;
  return arc->read(arc, dest, pos, length);
}

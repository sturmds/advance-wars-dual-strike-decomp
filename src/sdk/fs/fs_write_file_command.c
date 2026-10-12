#include "sdk/fs.h"

/* Archive command: writes the requested bytes at the file position through
 * the archive's write routine and moves the position past them. */
int FSi_WriteFileCommand(FSFile* file) {
  u32 pos = file->pos;
  u32 length = file->arg.readFile.length;
  FSArchive* arc = file->arc;
  void* dest = file->arg.readFile.dest;

  file->pos = pos + length;
  return arc->write(arc, dest, pos, length);
}

#include "types.h"
#include "game/sound_arc.h"

/* Reads up to size bytes of file number fileNo of the current sound archive,
 * starting offset bytes into it, to dest. Returns the byte count read, or -1
 * when the file does not exist or the seek fails. */
s32 SoundArc_ReadFile(u32 fileNo, void* dest, u32 size, u32 offset) {
  SoundArc* arc = gCurrentSoundArc;
  SoundArcFileEntry* entry;
  u32 left;

  if (fileNo >= arc->files->count) {
    return -1;
  }
  entry = &arc->files->entries[fileNo];
  left = entry->size - offset;
  if (size > left) {
    size = left;
  }
  if (!FS_SeekFile(&arc->file, entry->offset + offset, 0)) {
    return -1;
  }
  return FS_ReadFile(&arc->file, dest, size);
}

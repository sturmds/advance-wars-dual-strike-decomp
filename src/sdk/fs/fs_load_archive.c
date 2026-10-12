#include "sdk/fs.h"

int FSi_ReadMemCallback(FSArchive* arc, void* dest, u32 src, u32 len);
int FSi_WriteMemCallback(FSArchive* arc, const void* src, u32 dest, u32 len);

/* Sets up an archive over the memory that starts at base: the FAT and FNT
 * locations and sizes, and the read and write routines (plain memory copies
 * when none are given). Marks the archive as loaded. */
BOOL FS_LoadArchive(FSArchive* arc, u32 base, u32 fat, u32 fatSize, u32 fnt,
                    u32 fntSize, ArchiveByteReader read,
                    ArchiveByteWriter write) {
  arc->base = base;
  arc->fatSize = fatSize;
  arc->fatOriginal = fat;
  arc->fat = arc->fatOriginal;
  arc->fntSize = fntSize;
  arc->fntOriginal = fnt;
  arc->fnt = arc->fntOriginal;
  arc->read = read != NULL ? read : FSi_ReadMemCallback;
  arc->write = write != NULL ? write : FSi_WriteMemCallback;
  arc->readTable = arc->read;
  arc->unk_38 = 0;
  arc->flags |= ARCHIVE_STATE_ROUTINES_SET;
  return TRUE;
}

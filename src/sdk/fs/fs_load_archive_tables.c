#include "sdk/fs.h"

void MI_CpuFill8(void* dest, int value, u32 size); /* looks like MI_CpuFill8 */
int func_0211257c(FSArchive* arc, void* dest, u32 src,
                  u32 length); /* copies from memory */

static inline u32 Archive_RoundUpTo32(u32 x) { return (x + 31) & ~31; }

/* Copies the archive's FAT and then its FNT into mem (aligned to 32 bytes)
 * when they fit in size bytes, and reads them from memory from then on.
 * Returns the size needed, whether or not it fit. A table that cannot be
 * read is left as zeros. */
u32 FS_LoadArchiveTables(FSArchive* arc, void* mem, u32 size) {
  /* needed to match: int needed and the int cast keep the two adds apart */
  int needed = Archive_RoundUpTo32((int)(arc->fatSize + arc->fntSize + 32));

  if (needed <= size) {
    u8* cache = (u8*)Archive_RoundUpTo32((u32)mem);
    FSFile file;

    FS_InitFile(&file);
    if (FS_OpenFileDirect(&file, arc, arc->fat, arc->fat + arc->fatSize,
                          (u32)-1)) {
      if (FS_ReadFile(&file, cache, arc->fatSize) < 0) {
        MI_CpuFill8(cache, 0, arc->fatSize);
      }
      FS_CloseFile(&file);
    }
    arc->fat = (u32)cache;
    cache += arc->fatSize;

    if (FS_OpenFileDirect(&file, arc, arc->fnt, arc->fnt + arc->fntSize,
                          (u32)-1)) {
      if (FS_ReadFile(&file, cache, arc->fntSize) < 0) {
        MI_CpuFill8(cache, 0, arc->fntSize);
      }
      FS_CloseFile(&file);
    }
    arc->fnt = (u32)cache;

    arc->unk_38 = (u32)mem; /* where the tables were loaded */
    arc->readTable = func_0211257c;
    arc->flags |= ARCHIVE_STATE_TABLES_CACHED;
  }
  return needed;
}

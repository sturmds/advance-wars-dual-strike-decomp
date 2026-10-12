#include "types.h"
#include "sdk/fs.h"
#include "sdk/mb.h"
#include "sdk/rom_header.h"

/* The buffer DownloadPlay_LoadChildProgram needs for a program: header, auth code, cache
 * list, card map and both static programs, at least 64 KB. file NULL means
 * the running program. Returns 0 when the header cannot be read. */
u32 DownloadPlay_ProgramBufferSize(FSFile* file) {
  const RomHeader* header = NULL;
  u32 length = 0;
  u8 headerStart[0x60]; /* the header up to arm7Size and beyond */

  if (file != NULL) {
    u32 position = file->pos - file->top;

    if (FS_ReadFile(file, headerStart, sizeof(headerStart)) >=
        sizeof(headerStart)) {
      header = (const RomHeader*)headerStart;
    }
    FS_SeekFile(file, position, FS_SEEK_SET);
  } else {
    header = ROM_HEADER_BUF;
  }
  if (header != NULL) {
    length = DLPLAY_PREFIX_BYTES + header->arm9Size + header->arm7Size;
    if (length < DLPLAY_PROGRAM_BUFFER_FLOOR) {
      length = DLPLAY_PROGRAM_BUFFER_FLOOR;
    }
  }
  return length;
}

#include "sdk/fs.h"

void FSi_ReadTable(ArchiveTableCursor* cursor, void* dest, u32 length);

/* Archive command: reads the next entry of an open directory from the FNT
 * into arg.readDir.entry and moves past it. Each entry starts with a byte
 * holding the name length (low 7 bits) and a directory flag (bit 7), then
 * the name, then for a directory its id as a halfword. Files take their id
 * from the directory's running index. Fails at the end of the listing. */
int Archive_CmdNextDirEntry(FSFile* file) {
  DirListingEntry* entry = file->arg.readDir.entry;
  u8 header;
  ArchiveTableCursor cursor;

  cursor.arc = file->arc;
  cursor.pos = file->dirPos.pos;
  FSi_ReadTable(&cursor, &header, 1);
  entry->nameLength = header & 0x7f;
  entry->isDirectory = (header >> 7) & 1;
  if (entry->nameLength == 0) {
    return FS_RESULT_FAILURE;
  }
  if (file->arg.readDir.skipName) {
    cursor.pos += entry->nameLength;
  } else {
    FSi_ReadTable(&cursor, entry->name, entry->nameLength);
    entry->name[entry->nameLength] = '\0';
  }
  if (entry->isDirectory) {
    u16 id;

    FSi_ReadTable(&cursor, &id, 2);
    entry->dirId.arc = file->arc;
    entry->dirId.ownId = id & 0xfff;
    entry->dirId.index = 0;
    entry->dirId.pos = 0;
  } else {
    entry->fileId.arc = file->arc;
    entry->fileId.file_id = file->dirPos.index;
    file->dirPos.index++;
  }
  file->dirPos.pos = cursor.pos;
  return FS_RESULT_SUCCESS;
}

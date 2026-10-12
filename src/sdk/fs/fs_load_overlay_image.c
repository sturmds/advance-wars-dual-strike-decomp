#include "sdk/fs.h"

BOOL FS_OpenFileFast(FSFile* file, FSFileID id);
u32 Overlay_FileReadSize(const FSOverlayInfo* info);
void Overlay_InvalidateAndZeroBss(const FSOverlayInfo* info);

/* Reads the overlay file into the overlay's memory. */
BOOL FS_LoadOverlayImage(const FSOverlayInfo* info) {
  FSFileID id;
  FSFile file;
  u32 size;

  FS_InitFile(&file);
  id = FS_GetOverlayFileID(info);
  if (!FS_OpenFileFast(&file, id)) {
    return FALSE;
  }
  size = Overlay_FileReadSize(info);
  Overlay_InvalidateAndZeroBss(info);
  if (FS_ReadFile(&file, info->header.ram_address, size) != size) {
    FS_CloseFile(&file);
    return FALSE;
  }
  FS_CloseFile(&file);
  return TRUE;
}

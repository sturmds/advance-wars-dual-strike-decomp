#include "sdk/fs.h"
#include "sdk/rom_header.h"

void Mem_CopyBytes(const void* src, void* dst, u32 size);
BOOL FS_OpenFileFast(FSFile* file, FSFileID id);
BOOL FSi_LoadOverlayInfoCore(FSOverlayInfo* info, MIProcessor target, u32 id,
                             FSArchive* arc, u32 arm9Offset, u32 arm9Length,
                             u32 arm7Offset, u32 arm7Length);

BOOL FS_LoadOverlayInfo(FSOverlayInfo* info, MIProcessor target, u32 id) {
  RomRegion* table =
      target == MI_PROCESSOR_ARM9 ? &data_022ab844 : &data_022ab84c;

  if (table->offset != 0) {
    u32 pos = id * sizeof(OverlayTableRow);
    if (pos >= table->length) {
      return FALSE;
    }
    Mem_CopyBytes((const void*)(table->offset + pos), info,
                  sizeof(OverlayTableRow));
    info->target = target;
    {
      FSFile file;
      FS_InitFile(&file);
      if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(info))) {
        return FALSE;
      }
      info->romTop = file.top;
      info->romSize = file.bottom - file.top;
      FS_CloseFile(&file);
    }
    return TRUE;
  }
  {
    RomRegion* arm9 = &ROM_HEADER_BUF->arm9Overlays;
    RomRegion* arm7 = &ROM_HEADER_BUF->arm7Overlays;
    return FSi_LoadOverlayInfoCore(info, target, id, &gRomFileArchive,
                                   arm9->offset, arm9->length, arm7->offset,
                                   arm7->length);
  }
}

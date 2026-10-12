#include "sdk/fs.h"

FSFileID FS_GetOverlayFileID(const FSOverlayInfo* info) {
  FSFileID id;

  id.arc = &gRomFileArchive;
  id.file_id = info->header.file_id;
  return id;
}

#include "sdk/fs.h"

/* Bytes of the overlay file to read: the compressed size when the overlay
 * table marks it compressed, else the size in memory. */
u32 Overlay_FileReadSize(const FSOverlayInfo* info) {
  if (info->header.flag & OVERLAY_STORED_PACKED) {
    return info->header.compressed;
  }
  return info->header.ram_size;
}

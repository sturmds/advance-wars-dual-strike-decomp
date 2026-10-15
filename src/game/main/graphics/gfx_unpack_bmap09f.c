#include "types.h"
#include "sdk/fs.h"

u32 File_UnpackTo(void* dest, const char* path);

extern char data_021587d0[]; /* "/bmap" */
extern char data_021587dc[]; /* "09f" */

/* Unpacks file /bmap/09f (window frame pieces and a few letters) to dst. */
u32 Gfx_UnpackBmap09f(void* dst) {
  FS_ChangeDir(data_021587d0);
  return File_UnpackTo(dst, data_021587dc);
}

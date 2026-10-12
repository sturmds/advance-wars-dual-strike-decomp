#include "types.h"
#include "sdk/fs.h"
#include "sdk/mb.h"

/* Reads an icon file of exactly the expected size into the game entry: the
 * 0x200-byte characters when isChars, else the 0x20-byte palette. */
BOOL MbGame_LoadIconFile(const char* path, DlPlayGameEntry* game,
                         BOOL isChars) {
  FSFile file;
  u8* dst = game->iconPalette;
  u32 size = isChars ? DLPLAY_ICON_PIXEL_BYTES : DLPLAY_ICON_COLOR_BYTES;

  if (isChars) {
    dst = game->iconChars;
  }
  if (path == NULL) {
    return FALSE;
  }
  FS_InitFile(&file);
  if (!FS_OpenFile(&file, path)) {
    return FALSE;
  }
  if (size != file.bottom - file.top) {
    FS_CloseFile(&file);
    return FALSE;
  }
  FS_ReadFile(&file, dst, size);
  FS_CloseFile(&file);
  return TRUE;
}

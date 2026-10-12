#include "types.h"
#include "game/icon.h"

const char* File_LocalizePath(const char* path);
void File_ReadAt(void* dest, const char* path, u32 offset,
                 u32 size); /* reads size bytes at offset of a file */

/* Loads the tiles of icon iconId to tile number tile of vram, outside the
 * bank slots. */
void Icon_LoadTo(int iconId, u8* vram, int tile) {
  int bank;
  const char* path;

  bank = Icon_GetBank(iconId);
  path = File_LocalizePath(gIconBankFiles[bank]);
  File_ReadAt(vram + tile * 32, path, Icon_GetFileOffset(iconId, bank),
              gIconSize[iconId][0] * gIconHeights[iconId][0] * 32);
}

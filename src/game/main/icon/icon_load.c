#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

const char* File_LocalizePath(const char* path);
void File_ReadAt(void* dest, const char* path, u32 offset,
                 u32 size); /* reads size bytes at offset of a file */

extern Unk_027c0280* data_027c0280;

/* Loads icon iconId into the next free slot of its bank's VRAM area. */
void Icon_Load(int iconId) {
  int bankId;
  u32 tile;
  u32 tiles;
  IconBank* bank;
  u8* dest;
  const char* path;

  bankId = Icon_GetBank(iconId);
  bank = &data_027c0280->iconBanks[bankId];
  tile = bank->slots[bank->count].tile;
  tiles = gIconSize[iconId][0] * gIconHeights[iconId][0];
  dest = bank->vram + tile * 32;
  path = File_LocalizePath(gIconBankFiles[bankId]);
  File_ReadAt(dest, path, Icon_GetFileOffset(iconId, bankId), tiles * 32);
  bank->slots[bank->count].iconId = iconId;
  bank->count++;
  if (bank->count < ICON_BANK_SLOTS) {
    bank->slots[bank->count].tile = tile + tiles;
  }
}

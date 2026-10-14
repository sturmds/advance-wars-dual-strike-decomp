#include "types.h"
#include "game/oam_cells.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

extern u8 data_02139208[];
extern const u32 data_02139228[];
extern const u32 data_02139248[];
extern const u32 data_02139268[];
extern const u32 data_02139288[];
extern const u32 data_021392a8[];
extern u8 data_027c0028;
extern u8 data_027c0058[];
extern u32 data_027c00a8[];
extern u32 data_027c00c8[];
extern u32 data_027c00e8[];
extern u32 data_027c0198[];
extern u32 data_027c0118[];
extern u8 data_027c0038[];
extern u8 data_027c0040;
extern u32 data_027c0078[];
extern u8 data_027c0060[];
extern u32 data_027c00f8[];
extern u8 data_027c00b8[];
extern u32 data_027c015c[];
extern u8 data_027c0108[];
extern u8 data_027c01ac[];
extern u32 data_027c0138[];
extern u8 data_027c0048[];
extern u32 data_027c0088[];
extern u8 data_027c00d8;
extern u32 data_027c0170[];
extern u8 data_027c01dc;
extern u8 data_027c0030;
extern u32 data_027c0068[];

char data_02158988[] = {
#include "data/main/data_02158988.inc.c"
};
char data_02158974[] = {
#include "data/main/data_02158974.inc.c"
};
char data_02158960[] = {
#include "data/main/data_02158960.inc.c"
};
char data_021589c4[] = {
#include "data/main/data_021589c4.inc.c"
};
char data_021589b0[] = {
#include "data/main/data_021589b0.inc.c"
};
char data_0215899c[] = {
#include "data/main/data_0215899c.inc.c"
};
const char* gIconBankFiles[ICON_BANK_COUNT] = {
    (char*)&data_021589b0, (char*)&data_021589c4, (char*)&data_02158988,
    (char*)&data_0215899c, (char*)&data_02158960, (char*)&data_02158974};
IconBankInfo gIconBankInfo[ICON_BANK_COUNT] = {
#include "data/main/gIconBankInfo.inc.c"
};
/* OBJ shape for a sprite of [height][width] tiles */
void* gObjShapeBySize[][17] = {0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c0028,
                               &data_027c0058,
                               &data_027c00a8,
                               &data_027c0020,
                               &data_027c00c8,
                               &data_027c00e8,
                               &data_027c0198,
                               &data_027c0118,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c0038,
                               &data_027c0040,
                               &data_027c0078,
                               &data_027c0060,
                               &data_027c00f8,
                               &data_027c00b8,
                               &data_027c015c,
                               &data_027c0108,
                               0,
                               &data_027c01ac,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c0010,
                               &data_027c0000,
                               &data_027c0138,
                               &data_027c0048,
                               &data_027c0088,
                               &data_027c00d8,
                               &data_027c0170,
                               &data_027c0008,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c01dc,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c0030,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               &data_027c0068};
/* per icon: width, height in tiles */
u8 gIconSize[ICON_COUNT][2] = {
#include "data/main/gIconSize.inc.c"
};

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

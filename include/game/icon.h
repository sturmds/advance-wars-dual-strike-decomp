#ifndef GAME_ICON_H
#define GAME_ICON_H

#include "types.h"

/* Icons: small OBJ pictures in six banks, one file per bank
 * (/icon/res_icon0_cg to /icon/res_icon5_cg). An icon id picks the bank,
 * see Icon_GetBank. */
#define ICON_COUNT 336
#define ICON_BANK_COUNT 6
#define ICON_BANK_SLOTS 32 /* icons one bank can hold in VRAM at once */

/* The digits 0-9, icon ids ICON_DIGITS to ICON_DIGITS + ICON_DIGIT_COUNT - 1. */
#define ICON_DIGITS 140
#define ICON_DIGIT_COUNT 10

/* An icon loaded into a bank's VRAM area. */
typedef struct IconSlot {
  u16 tile; /* first OBJ tile, from the bank's vram */
  u16 iconId;
} IconSlot;

/* VRAM area of one icon bank, Unk_027c0280.iconBanks[bank]. */
typedef struct IconBank {
  u8* vram;   /* 32 bytes per tile */
  u8 palette; /* OBJ palette slot */
  u8 count;   /* icons loaded */
  IconSlot
      slots[ICON_BANK_SLOTS]; /* slots[count].tile: where the next icon goes */
  u8 pad_86[0x88 - 0x86];
} IconBank;

/* gIconBankInfo[bank]. firstIcon is read through gIconBankFirstIcon. */
typedef struct IconBankInfo {
  const u16* palette; /* 16 colours */
  u16 firstIcon;
  u8 pad_06[2];
} IconBankInfo;

extern IconBankInfo gIconBankInfo[ICON_BANK_COUNT];
extern u8 gIconSize[ICON_COUNT][2]; /* per icon: width, height in tiles */
extern u8 gIconHeights[ICON_COUNT]
                      [2]; /* gIconSize one byte on: [i][0] is the height */
extern u16 gIconBankFirstIcon[]; /* first icon id of each bank, every 4th u16 */
extern const char* gIconBankFiles[ICON_BANK_COUNT];

int Icon_GetBank(int iconId);
u32 Icon_GetFileOffset(int iconId, int bank);
void Icon_Draw(int iconId, int x, int y, int tile, int list);
void Icon_Load(int iconId);
void Icon_LoadTo(int iconId, u8* vram, int tile);
void Icon_LoadPalette(int iconId, int paletteSlot);
void IconBank_LoadPalette(int bank, int paletteSlot);
void IconBank_Init(int bank, u8* vram, int firstTile, int paletteSlot);

#endif

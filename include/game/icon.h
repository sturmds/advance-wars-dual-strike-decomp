#ifndef GAME_ICON_H
#define GAME_ICON_H

#include "types.h"

/* Icons: small OBJ pictures in six banks, one file per bank
 * (/icon/res_icon0_cg to /icon/res_icon5_cg). An icon id picks the bank,
 * see Icon_GetBank. */
#define ICON_COUNT 336
#define ICON_BANK_COUNT 6
#define ICON_BANK_SLOTS 32 /* icons one bank can hold in VRAM at once */

/* Icon ids. A run holds one icon per value: its first id plus the value
 * gives the icon. docs/icons.md shows the pictures and the evidence. */
enum IconId {
  ICON_ARMY_BANNERS = 75,     /* by army colour - 1: emblem with a dog, 4x2 */
  ICON_ARMY_EMBLEMS = 80,     /* by army colour - 1: the emblem alone, 2x2 */
  ICON_DIGITS_OUTLINED = 108, /* 0-9, white with a black outline, 1x2 */
  ICON_DIGITS_GREY = 120,     /* 0-9, grey, 1x2 */
  ICON_RANK_LABEL = 139,      /* "RANK" */
  ICON_DIGITS = 140,          /* 0-9, white, 1x2 */
  ICON_DIGITS_SMALL = 150,    /* 0-9, yellow, 1x1 */
  ICON_SMALL_SLASH = 162,     /* "/" beside ICON_DIGITS_SMALL */
  ICON_PLACES = 188,          /* "1st" to "5th" */
  ICON_DAY_LABEL = 200,       /* "DAY" */
  ICON_PTS_LABEL = 201,       /* "PTS" */
  ICON_PLAYER_LABELS = 204,   /* "P1" to "P4" */
  ICON_CPU_LABEL = 208,       /* "CPU" */
  ICON_STAT_GAUGE = 225, /* a gauge for a stat change of -20 to +20 steps */
  ICON_DIGITS_SMALL_WHITE = 266, /* 0-9, white, 1x1 */
  ICON_TEAMS = 276,              /* "A TEAM" to "D TEAM" */
  ICON_SETTING_LABELS = 280,     /* Vs settings: "Fog" to "CPU AI", by row */
  ICON_ON = 289,
  ICON_OFF = 290,
  ICON_VISUAL_TYPES = 291,    /* "TYPE A" to "TYPE D" */
  ICON_SETTING_CIRCLES = 295, /* Vs settings: a coloured disc per row */
  ICON_WEATHERS = 304,        /* Vs settings: weather pictures */
  ICON_TILESETS = 309,        /* Vs settings: land pictures by map tileset */
  ICON_AI_KINDS = 313,        /* Vs settings: CPU AI pictures (guess) */
  ICON_DIGITS_SETUP = 317,    /* 0-9, white, 1x2, in bank 5 */
  ICON_TEAM_LETTERS = 327,    /* "a" to "d" in the team colours */
  ICON_TEAM_LETTERS_GREY = 331
};

/* Icons in each run above. */
#define ICON_DIGIT_COUNT 10
#define ICON_PLACE_COUNT 5
#define ICON_PLAYER_LABEL_COUNT 4
#define ICON_STAT_GAUGE_COUNT 41 /* -20 to +20 */
#define ICON_TEAM_COUNT 4
#define ICON_SETTING_COUNT 9 /* rows of the Vs settings menu */
#define ICON_VISUAL_TYPE_COUNT 4
#define ICON_WEATHER_COUNT 5
#define ICON_TILESET_COUNT 4
#define ICON_AI_KIND_COUNT 4

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

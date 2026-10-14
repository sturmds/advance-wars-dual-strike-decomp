#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"

extern MenuState gMenuState;
extern Unk_027c0280* data_027c0280;
static char Ov002_gBattleResPath[] = {
#include "data/ov002/Ov002_gBattleResPath.inc.c"
};
static char Ov002_gBattleLangResPath[] = {
#include "data/ov002/Ov002_gBattleLangResPath.inc.c"
};

void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Mem_Free(void* block);
u32 Battle_PickHappyQuote(int side);
u32 Battle_PickAngryQuote(int side);
s32 _s32_div_f(s32 num, s32 den);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Gfx_ClearLayerMap(int a);
void Gfx_RequestLayerCopy(int a);
void Data_Unpack(const void* src, void* dst);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Co_LoadMoodPicture(u32 a, void* dst, int b, int c);
void Battle_PickFrame(void);
BOOL Battle_IsSideAir(int side);
int Battle_FindArmySlot(int army);
u8 Battle_GetSideMood(u8 side);
u8 Battle_GetStartMood(u8 side);
void Battle_BuildSideTilemaps(void* buf);

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

#define CHAR_BASE(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Sets up the battle animation screen: the HP drain of each side, the
 * frame graphics picked by data_0229fb30.unk_03, each CO's happy or angry
 * quote, and the CO portraits with their army palettes. The defender's bar
 * always drains, the attacker's only when the defender survives and hits
 * back. */
void Battle_ShowQuote(void) {
  int i;
  u8* lang;
  u8* res;
  s16 hpLost;
  int tile;
  int palette;
  u8 bg;
  u8 bg2;

  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    gMenuState.battle.hpStart[i] = Ov002_gBattleScene->sides[i].hpBefore;
    gMenuState.battle.unk_1000[i] = 0;
    gMenuState.battle.hpShown[i] = gMenuState.battle.hpStart[i] << 16;
    hpLost = Ov002_gBattleScene->sides[i].hpBefore -
             Ov002_gBattleScene->sides[i].hpAfter;
    /* Needed to match: the frame count is written out twice, not kept
         * in a local. The drain takes 20 frames plus one per 5 HP lost. */
    gMenuState.battle.hpStep[i] = _s32_div_f(hpLost << 16, hpLost / 5 + 20);
    gMenuState.battle.unk_1008[i] = 0;
    if (i != Ov002_gBattleScene->attackerSide ||
        (Ov002_gBattleScene->sides[(i + 1) & 1].hpAfter != 0 &&
         Ov002_gBattleScene->sides[(i + 1) & 1].counters != 0)) {
      gMenuState.battle.hpFrames[i] = hpLost / 5 + 20;
    } else {
      gMenuState.battle.hpFrames[i] = 0;
    }
  }

  data_0229fb30.unk_02 = 1;
  Mem_Zero32(0, BATTLE_FIELD(unk_7d0), sizeof(gMenuState.battle.unk_7d0));
  Gfx_ClearLayerMap(1);
  res = File_Load(Ov002_gBattleResPath);
  lang = File_Load(File_LocalizePath(Ov002_gBattleLangResPath));
  Battle_PickFrame();

  switch (data_0229fb30.unk_03) {
    case 1:
      Data_Unpack(lang, data_027c0280->objVram);
      Mem_Copy32(res + 0xbf0, BATTLE_FIELD(unk_fd0),
                 sizeof(gMenuState.battle.unk_fd0));
      break;
    case 2:
      Data_Unpack(lang + 0x35c, data_027c0280->objVram);
      Mem_Copy32(res + 0xc10, BATTLE_FIELD(unk_fd0),
                 sizeof(gMenuState.battle.unk_fd0));
      break;
    case 3:
      Data_Unpack(lang + 0x6cc, data_027c0280->objVram);
      Mem_Copy32(res + 0xc30, BATTLE_FIELD(unk_fd0),
                 sizeof(gMenuState.battle.unk_fd0));
      break;
  }
  Palette_Load(res + 0xbd0, 0x200, 32);

  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    gMenuState.battle.mood[i] = Battle_GetSideMood(i);
    switch (gMenuState.battle.mood[i]) {
      case 0:
        break;
      case 1:
        tile = i == 0 ? 0x100 : 0x120;
        Data_Unpack(lang + Battle_PickHappyQuote(i),
                    data_027c0280->objVram + tile * 32);
        break;
      case 2:
        tile = i == 0 ? 0x100 : 0x120;
        Data_Unpack(lang + Battle_PickAngryQuote(i),
                    data_027c0280->objVram + tile * 32);
        break;
    }
  }

  /* Needed to match: the original turns each result into 0 or 1 first. */
  if (!(Battle_IsSideAir(0) ? TRUE : FALSE)) {
    Data_Unpack(lang + 0xa80, data_027c0280->objVram + 0x1800);
  } else {
    Data_Unpack(lang + 0xc58, data_027c0280->objVram + 0x1800);
  }
  if (!(Battle_IsSideAir(1) ? TRUE : FALSE)) {
    Data_Unpack(lang + 0xb6c, data_027c0280->objVram + 0x1c00);
  } else {
    Data_Unpack(lang + 0xd40, data_027c0280->objVram + 0x1c00);
  }

  Data_Unpack(res, CHAR_BASE(data_027c0280->layer1Bg));
  Mem_Copy32(res + 0x828, BATTLE_FIELD(unk_490),
             sizeof(gMenuState.battle.unk_490));
  Mem_Copy32(res + 0x88c, BATTLE_FIELD(unk_510),
             sizeof(gMenuState.battle.unk_510));
  Mem_Copy32(res + 0xb54, BATTLE_FIELD(unk_750),
             sizeof(gMenuState.battle.unk_750));

  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    BOOL left = i == 0 ? TRUE : FALSE;

    palette = left ? 10 : 11;
    bg = data_027c0280->layer1Bg;
    tile = left ? 304 : 334;
    Co_LoadMoodPicture(
        Ov002_gBattleScene->sides[i].coId | (Battle_GetStartMood(i) << 8),
        CHAR_BASE(bg) + tile * 32, palette,
        Battle_FindArmySlot(Ov002_gBattleScene->sides[i].army));
    palette = left ? 10 : 11;
    bg2 = data_027c0280->layer1Bg;
    tile = left ? 364 : 394;
    Co_LoadMoodPicture(
        Ov002_gBattleScene->sides[i].coId | (Battle_GetSideMood(i) << 8),
        CHAR_BASE(bg2) + tile * 32, palette,
        Battle_FindArmySlot(Ov002_gBattleScene->sides[i].army));
    Palette_Load(res + 0xab4 + (Ov002_gBattleScene->sides[i].army - 1) * 32,
                 (u16)((left ? 8 : 9) * 32),
                 32); /* the u16 cast is needed to match */
  }

  Mem_Free(res);
  Mem_Free(lang);
  Battle_BuildSideTilemaps(BATTLE_FIELD(unk_7d0));
  Gfx_RequestLayerCopy(1);
}

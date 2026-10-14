#include "types.h"
#include "game/icon.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_022a7a9c.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/win_lose.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Gfx_ClearLayerMap(int layer);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
int Campaign_IsHardMode(void);
int CoExp_IsSingleColumn(void);
void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Gfx_RequestLayerCopy(int layer);
void Co_LoadSmallPicture(int coId, void* dest, int paletteSlot);
void Co_UnpackPicture(int coId, int firstTile);
void Palette_LoadObjRow(int row);
void Mem_Copy32(const void* src, void* dst, u32 size);
s32 HwMod32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern u8 data_022a0030[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAIN_MAP (data_027c0288.unk_00)
#define ENTRY(player) ((&data_022a7a9c)[player])
#define ROWS (((BattleResults*)&data_022a7a9c)->rows)
#define EXP gMenuState.coExp.exp

/* Screen graphics an earlier step unpacked over gMenuState: tile blocks at
 * 0x000, 0x16c and 0x218 for layer 1, then 0x1100 bytes of OBJ tiles at
 * 0x264. */
#define RES ((u8*)&gMenuState)

/* Sets up the CO experience page of the results screen on the main screen.
 * Draws the panel tile blocks on layer 1 (one centered panel on campaign
 * maps up to 231 on normal mode, and in the War Room when CoExp_IsSingleColumn
 * says 1; else one panel per side A player, at most three). Loads each
 * player's army icon and the small pictures of the front and back CO,
 * two rows of ten icons (likely digits), and the experience multiplier
 * icon: War Room unk_59 picks 187-184, other modes 183. Then fills
 * gMenuState.coExp.exp from the rank and experience each CO had before
 * the battle; a missing back CO gets level -1. */
void CoExp_InitScreen(void) {
  int i;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(MAIN_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(MAIN_MAP);
  Gfx_ClearLayerMap(1);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0xff00;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  if (GAME->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode() &&
      GAME->unk_1c4->mapId <= 231) {
    Bg_PutTileBlock(data_027c0280->layer1Map, 9, 8, RES + 0x16c, 0);
  } else if (GAME->gameMode == GAME_MODE_WAR_ROOM) {
    if (CoExp_IsSingleColumn() == 1) {
      Bg_PutTileBlock(data_027c0280->layer1Map, 9, 6, RES + 0x16c, 0);
      Bg_PutTileBlock(data_027c0280->layer1Map, 13, 12, RES + 0x218, 0);
    } else {
      Bg_PutTileBlock(data_027c0280->layer1Map, 2, 6, RES, 0);
      Bg_PutTileBlock(data_027c0280->layer1Map, 13, 12, RES + 0x218, 0);
    }
  } else {
    switch (data_022a7a88.countA) {
      case 1:
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 8, RES, 0);
        break;
      case 2:
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 5, RES, 0);
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 12, RES, 0);
        data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 4;
        break;
      default:
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 1, RES, 0);
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 8, RES, 0);
        Bg_PutTileBlock(data_027c0280->layer1Map, 2, 15, RES, 0);
        break;
    }
  }
  Gfx_RequestLayerCopy(1);

  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i >= 3) {
      break;
    }
    Icon_LoadTo(ICON_ARMY_BANNERS - 1 + ROWS[data_022a7a88.sideA[i]].armyColour,
                data_027c0280->objVram, 756 + i * 8);
    Co_LoadSmallPicture(ENTRY(data_022a7a88.sideA[i]).frontCo,
                        data_027c0280->objVram + ((548 + i * 24) << 5),
                        21 + i * 2);
    Co_UnpackPicture(ENTRY(data_022a7a88.sideA[i]).frontCo & PLAYER_CO_ID_MASK,
                     476 + i * 24);
    if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {

      Co_LoadSmallPicture(
          ENTRY(data_022a7a88.sideA[i]).backCo,
          data_027c0280->objVram + (((i * 2 + 1) * 12 + 548) << 5), i * 2 + 22);
      Co_UnpackPicture(ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK,
                       (i * 2 + 1) * 12 + 476);
    }
  }

  Palette_LoadObjRow(4);
  IconBank_LoadPalette(1, 27);
  IconBank_LoadPalette(4, 28);
  Icon_LoadTo(ICON_RANK_LABEL, data_027c0280->objVram, 780);
  for (i = 0; i < ICON_DIGIT_COUNT; i++) {
    Icon_LoadTo(ICON_DIGITS + i, data_027c0280->objVram, 788 + i * 2);
  }
  Icon_LoadTo(213, data_027c0280->objVram, 808);
  Icon_LoadTo(182, data_027c0280->objVram, 812);
  for (i = 0; i < ICON_DIGIT_COUNT; i++) {
    Icon_LoadTo(ICON_DIGITS_SMALL + i, data_027c0280->objVram, 816 + i * 2);
  }
  if (GAME->gameMode == GAME_MODE_WAR_ROOM) {
    if (GAME->unk_59 == 0) {
      Icon_LoadTo(187, data_027c0280->objVram, 836);
    } else if (GAME->unk_59 == 1) {
      Icon_LoadTo(186, data_027c0280->objVram, 836);
    } else if (GAME->unk_59 == 2) {
      Icon_LoadTo(185, data_027c0280->objVram, 836);
    } else {
      Icon_LoadTo(184, data_027c0280->objVram, 836);
    }
  } else {
    Icon_LoadTo(183, data_027c0280->objVram, 836);
  }
  Mem_Copy32(RES + 0x264, data_027c0280->objVram + 0x4d80, 0x1100);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  /* The result rows are read by i + 1 here, not by side A's player slot. */
  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i >= 3) {
      break;
    }
    EXP[i * 2].level = ROWS[i + 1].rankBefore[0];
    EXP[i * 2].points = HwMod32(ROWS[i + 1].expInRank[0], CO_EXP_PER_LEVEL);
    if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {
      EXP[i * 2 + 1].level = ROWS[i + 1].rankBefore[1];
      EXP[i * 2 + 1].points =
          HwMod32(ROWS[i + 1].expInRank[1], CO_EXP_PER_LEVEL);
    } else {
      EXP[i * 2 + 1].level = -1;
      EXP[i * 2 + 1].points = -1;
    }
    EXP[i * 2].levelUpFrame = 0;
    EXP[i * 2 + 1].levelUpFrame = 0;
  }

  if (GAME->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode() &&
      GAME->unk_1c4->mapId <= 231) {
    /* exp[1] gets level 0 and 0 points, back CO or not. Needed to
         * match: written through the symbols at gMenuState + 0x1000 and + 0x1500. */
    ((u8*)&data_0229fb30)[offsetof(MenuState, coExp.exp[1].level) -
                          offsetof(MenuState, shared.state)] = 0;
    *(s16*)(data_022a0030 + 0xae) = 0;
  }
}

#include "types.h"
#include "game/task.h"
#include "game/second_screen.h"
#include "game/screen_other_map.h"
#include "game/co_data.h"
#include "game/player.h"

int IncreaseFunds(int playerIndex, int terrain);
BOOL func_ov000_022b2c64(void); /* GameFlag_IsSet(59) */
void Font_LoadDigits(int a);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Gfx_RequestLayerCopy(int layer);

void SecondScreen_RefreshIfChanged(void);
extern u8 data_ov000_022d5a0c[];
extern const u32 data_ov000_022d87ac[];
void func_ov000_022b2cb8(void);
void Layer0_ResetScroll(void);
extern const u32 data_ov000_022d5fa8[];
extern const u32 data_ov000_022d8cf4[];
extern const u32 data_ov000_022d5fc8[];
extern const u32 data_ov000_022d8d14[];
extern const u32 data_ov000_022d64d8[];
extern const u32 data_ov000_022d92d8[];
extern const u32 data_ov000_022d64f8[];
extern const u32 data_ov000_022d92f8[];
extern const u32 data_ov000_022d6ad0[];
extern const u32 data_ov000_022d98f4[];
extern u8 data_ov000_022d6af0[];
extern u8 data_ov000_022d9914[];
extern const u32 data_ov000_022d6f6c[];
extern const u32 data_ov000_022d9f88[];
extern u8 data_ov000_022d9fa8[];
extern const u32 data_ov000_022da5d4[];

/* first map entry of each row's 4x2 icon */
static u16 Ov000_gTerrainPanelMoveTypeTiles[] = {
#include "data/ov000/Ov000_gTerrainPanelMoveTypeTiles.inc.c"
};
/* movement chart rows shown, in order */
static int Ov000_gTerrainPanelMoveTypes[] = {
#include "data/ov000/Ov000_gTerrainPanelMoveTypes.inc.c"
};
TaskScriptCmd data_ov000_023500f8[] = {
#include "data/ov000/data_ov000_023500f8.inc.c"
};
/* tile x, y of the n-th icon shown */
static int Ov000_gTerrainPanelIconPos[][2] = {
#include "data/ov000/Ov000_gTerrainPanelIconPos.inc.c"
};
/* tile x, y pairs of the n-th cost shown */
static int Ov000_gTerrainPanelCostPos[] = {
#include "data/ov000/Ov000_gTerrainPanelCostPos.inc.c"
};
int gTerrainStarOffsets[] = {
#include "data/ov000/gTerrainStarOffsets.inc.c"
};
const void* data_ov000_023501e8[][6] = {
    &data_ov000_022d87ac, &data_ov000_022d8cf4, 0, 0, 0, 0,
    &data_ov000_022d87ac, &data_ov000_022d8cf4, 0, 0, 0, 0,
    &data_ov000_022d8d14, &data_ov000_022d92d8, 0, 0, 0, 0,
    &data_ov000_022d92f8, &data_ov000_022d98f4, 0, 0, 0, 0,
    &data_ov000_022d9914, &data_ov000_022d9f88, 0, 0, 0, 0,
    &data_ov000_022d9fa8, &data_ov000_022da5d4, 0, 0, 0, 0};
const void* data_ov000_02350278[][6] = {&data_ov000_022d5a0c,
                                        &data_ov000_022d5fa8,
                                        0,
                                        0,
                                        0,
                                        0,
                                        &data_ov000_022d5a0c,
                                        &data_ov000_022d5fa8,
                                        0,
                                        0,
                                        0,
                                        0,
                                        &data_ov000_022d5fc8,
                                        &data_ov000_022d64d8,
                                        0,
                                        0,
                                        0,
                                        0,
                                        &data_ov000_022d64f8,
                                        &data_ov000_022d6ad0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        &data_ov000_022d6af0,
                                        &data_ov000_022d6f6c,
                                        0,
                                        0,
                                        0,
                                        0,
                                        &data_ov000_022d5a0c,
                                        &data_ov000_022d5fa8,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0};

/* Needed to match: reads through a cast pointer make the compiler load the icon
 * position again after every map store, as the original does. */
#define ICON_X(n) ((int*)Ov000_gTerrainPanelIconPos)[(n) * 2]
#define ICON_Y(n) ((int*)Ov000_gTerrainPanelIconPos)[(n) * 2 + 1]

#define PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Second screen, terrain panel at pixel x: draws the funds the terrain gives the player
 * in data_ov000_0235054c (IncreaseFunds) when not 0, then for each of nine movement
 * chart rows a 4x2 icon and the cost of moving onto the terrain. The costs come from the
 * current player's chart: front CO, power state and Player.moveChart (CO id 0 when
 * coEffectsOn is 0). Rows with cost -1 (cannot enter) are skipped and the next one takes
 * their place. Row 8 is skipped unless game flag 59 is set. */
void SecondScreen_DrawTerrainMoveCosts(u8 x, u8 terrain) {
  int funds;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  u16 i;
  u16 n;
  int co;
  int costs[ARRAY_COUNT(Ov000_gTerrainPanelMoveTypes)];

  n = 0;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  funds = IncreaseFunds(data_ov000_0235054c, terrain);
  for (i = 0; i < (int)ARRAY_COUNT(Ov000_gTerrainPanelMoveTypes); i++) {
    if (func_ov000_022b2c64() == 0 && Ov000_gTerrainPanelMoveTypes[i] == 8) {
      costs[i] = -1;
      continue;
    }
    if (MAP_STATE->coEffectsOn == 0) {
      co = 0;
    } else {
      co = PLAYER.front.id & PLAYER_CO_ID_MASK;
    }
    costs[i] =
        ((const s8(*)[32])gCoTable[co]
             .blocks[PLAYER.front.powerState]
             .movementCost[PLAYER.moveChart])[Ov000_gTerrainPanelMoveTypes[i]]
                                             [terrain];
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  Font_LoadDigits(0);
  if (funds != 0) {
    Number_Draw((x + 96) / 8, 5, data_027c0280->layer0Map, funds, 0, 0);
  }
  for (i = 0; i < (int)ARRAY_COUNT(Ov000_gTerrainPanelMoveTypes); i++) {
    if (costs[i] != -1) {
      u16* map = data_027c0280->layer0Map;
      (map + (x + ICON_X(n)))[ICON_Y(n) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i];
      (map + (x + ICON_X(n) + 1))[ICON_Y(n) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 1;
      (map + (x + ICON_X(n) + 2))[ICON_Y(n) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 2;
      (map + (x + ICON_X(n) + 3))[ICON_Y(n) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 3;
      (map + (x + ICON_X(n)))[(ICON_Y(n) + 1) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 4;
      (map + (x + ICON_X(n) + 1))[(ICON_Y(n) + 1) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 5;
      (map + (x + ICON_X(n) + 2))[(ICON_Y(n) + 1) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 6;
      (map + (x + ICON_X(n) + 3))[(ICON_Y(n) + 1) * 32] =
          Ov000_gTerrainPanelMoveTypeTiles[i] + 7;
      Number_Draw((x + Ov000_gTerrainPanelCostPos[n * 2] * 8) / 8,
                  Ov000_gTerrainPanelCostPos[n * 2 + 1],
                  data_027c0280->layer0Map, costs[i], 0, 0);
      n++;
    }
  }
  Gfx_RequestLayerCopy(0);
}

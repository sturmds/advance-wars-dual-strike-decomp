#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"
#include "game/Unk_022a7b80.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/win_lose.h"

void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void* File_Load(const char* path);
void Mem_Free(void* file);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void Gfx_RequestLayerCopy(int layer);
void Co_LoadArmyLargePicture(int coMood, void* dest, int paletteSlot, int army);
void NumberFont_LoadLow(void);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
void Menu_Reset(void);
void Wallpaper_LoadBoth(void);
void Task_JumpToLabel(Task* task, int label);
int WinLose_GetScoreWidth(int points);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern Unk_022a7b80 data_022a7b80;
extern const char gResultPointObjPath[]; /* "/ohashi/res_result_point_obj" */
extern const char gWinLosePath[];        /* "/ohashi/res_winlose" */

#define GAME (data_027c027c->unk_48[data_027c0284])

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* SWITCH_TO and SWITCH_BOTH open a block with their own saved pointers,
 * which SWITCH_BACK restores and closes. Needed to match: every switch
 * keeps its saved pointers in its own stack slots. */
#define SWITCH_TO(state)                     \
  {                                          \
    Unk_027c0280* saved0280 = data_027c0280; \
    Unk_02080440* saved027c;                 \
    Screen_SetCurrentGfx(state);             \
    saved027c = data_027c027c;               \
    Screen_SetCurrent(state)

#define SWITCH_BACK()              \
  Screen_SetCurrent(saved027c);    \
  Screen_SetCurrentGfx(saved0280); \
  }

#define SWITCH_BOTH(state)                   \
  {                                          \
    Unk_027c0280* saved0280 = data_027c0280; \
    Unk_02080440* saved027c = data_027c027c; \
    Screen_SetCurrentBoth(state)

/* Result entry of a player slot. */
#define ENTRY(player) ((&data_022a7a9c)[player])

/* Where the CO picture in `slot` goes in OBJ VRAM: 36 tiles of 32 bytes each. */
#define PORTRAIT_DEST(slot) (data_027c0280->objVram + ((slot) * 36 << 5))

/* First step of the win/lose screen's task (graphics from
 * /ohashi/res_result_point_obj and /ohashi/res_winlose). Starts both
 * screens at full white with layer 2 alpha-blended over layer 3 (first fully transparent), and loads the CO
 * pictures of the players in data_022a7a88: with two sides, side A on the
 * second screen and side B on the main one; with one side, its first two
 * players on the second screen and the rest on the main one. Bits 8-9 of
 * the CO id passed to Co_LoadArmyLargePicture pick the picture variant. With two
 * sides it counts the pictures and works out the points shown; with one
 * side it marks the players with a tag partner and jumps to label 1. */
void WinLoseScreen_Init(WinLoseTask* task) {
  int i;
  int slot; /* CO picture number: its OBJ VRAM place and palette 16 + slot */
  u8* file;

  Gfx_InitBothScreens();

  SWITCH_BOTH(MAIN_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
  Screen_SetCurrentBoth(SECOND_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
  SWITCH_BACK();

  SWITCH_BOTH(MAIN_MAP);
  data_027c0280->masterBrightness = 16;
  Screen_SetCurrentBoth(SECOND_MAP);
  data_027c0280->masterBrightness = 16;
  SWITCH_BACK();

  if (data_022a7a88.countB != 0) {
    slot = 0;
    SWITCH_TO(SECOND_MAP);
    for (i = 0; i < data_022a7a88.countA; i++) {
      Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).frontCo | 0x100,
                              PORTRAIT_DEST(slot), slot + 16,
                              data_022a7a88.sideA[i]);
      slot++;
      if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {
        Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).backCo | 0x100,
                                PORTRAIT_DEST(slot), slot + 16,
                                data_022a7a88.sideA[i]);
        slot++;
      }
    }
    SWITCH_BACK();

    slot = 0;
    SWITCH_TO(MAIN_MAP);
    for (i = 0; i < data_022a7a88.countB; i++) {
      Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideB[i]).frontCo | 0x200,
                              PORTRAIT_DEST(slot), slot + 16,
                              data_022a7a88.sideB[i]);
      slot++;
      if (ENTRY(data_022a7a88.sideB[i]).backCo & PLAYER_CO_ID_MASK) {
        Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideB[i]).backCo | 0x200,
                                PORTRAIT_DEST(slot), slot + 16,
                                data_022a7a88.sideB[i]);
        slot++;
      }
    }
    SWITCH_BACK();
  } else {
    for (i = 0; i < data_022a7a88.countA; i++) {
      if (i < 2) {
        SWITCH_TO(SECOND_MAP);
        Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).frontCo | 0x200,
                                PORTRAIT_DEST(i * 2), i * 2 + 16,
                                data_022a7a88.sideA[i]);
        if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {
          Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).backCo | 0x200,
                                  PORTRAIT_DEST(i * 2 + 1), i * 2 + 17,
                                  data_022a7a88.sideA[i]);
        }
        SWITCH_BACK();
      } else {
        SWITCH_TO(MAIN_MAP);
        Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).frontCo | 0x200,
                                PORTRAIT_DEST((i - 2) * 2), (i - 2) * 2 + 16,
                                data_022a7a88.sideA[i]);
        if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {
          Co_LoadArmyLargePicture(ENTRY(data_022a7a88.sideA[i]).backCo | 0x200,
                                  PORTRAIT_DEST((i - 2) * 2 + 1),
                                  (i - 2) * 2 + 17, data_022a7a88.sideA[i]);
        }
        SWITCH_BACK();
      }
    }
  }

  file = File_Load(gResultPointObjPath);
  SWITCH_TO(MAIN_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(file + 0xfc, data_027c0280->objVram + 0x1b00);
  Palette_Load(file + 0x988, 0x2c0, 0x20);
  SWITCH_BACK();
  Mem_Free(file);

  file = File_Load(gWinLosePath);
  SWITCH_TO(MAIN_MAP);
  Data_Unpack(
      file, data_027c0280->bgVram +
                (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Palette_Load(file + 0xcf0, 0x40, 0x20);
  SWITCH_BACK();
  SWITCH_TO(SECOND_MAP);
  Mem_Copy32(((Unk_027c0280*)MAIN_MAP)->unk_10c,
             data_027c0280->bgVram +
                 (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
             0x2000);
  Palette_Load(file + 0xcf0, 0x40, 0x20);
  SWITCH_BACK();
  if (data_022a7a88.countB != 0) {
    SWITCH_TO(SECOND_MAP);
    Data_Unpack(file + 0x6e0, data_027c0280->layer2Map);
    SWITCH_BACK();
    SWITCH_TO(MAIN_MAP);
    Data_Unpack(file + 0x884, data_027c0280->layer2Map);
    SWITCH_BACK();
  } else {
    SWITCH_TO(SECOND_MAP);
    Data_Unpack(file + 0xa04, data_027c0280->layer2Map);
    SWITCH_BACK();
    SWITCH_TO(MAIN_MAP);
    Data_Unpack(file + 0xb88, data_027c0280->layer2Map);
    SWITCH_BACK();
  }
  Data_Unpack(file + 0xd10, &gMenuState);
  /* Move layer 2's tile map entries up two palettes (bits 12-15). */
  SWITCH_BOTH(MAIN_MAP);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x2000;
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentBoth(SECOND_MAP);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x2000;
  }
  Gfx_RequestLayerCopy(2);
  SWITCH_BACK();
  Mem_Free(file);

  NumberFont_LoadLow();
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0xf800);
  Menu_Reset();
  Wallpaper_LoadBoth();

  task->frame = 0;
  if (data_022a7a88.countB != 0) {
    task->portraitsA = 0;
    for (i = 0; i < data_022a7a88.countA; i++) {
      task->portraitsA++;
      if (ENTRY(data_022a7a88.sideA[i]).backCo & PLAYER_CO_ID_MASK) {
        task->portraitsA++;
      }
    }
    task->portraitsB = 0;
    for (i = 0; i < data_022a7a88.countB; i++) {
      task->portraitsB++;
      if (ENTRY(data_022a7a88.sideB[i]).backCo & PLAYER_CO_ID_MASK) {
        task->portraitsB++;
      }
    }
    if (task->portraitsA > task->portraitsB) {
      task->portraitsMax = task->portraitsA;
    } else {
      task->portraitsMax = task->portraitsB;
    }
    if (GAME->gameMode == 3) { /* likely Survival */
      task->pointsTotal = data_022a7b80.points;
    } else {
      task->pointsAdded = data_022a7a88.unk_0c;
      task->pointsTotal = data_022a7a9c.unk_10;
    }
    if (task->pointsTotal > 99999) {
      task->pointsTotal = 99999;
    }
    task->scoreWidth =
        WinLose_GetScoreWidth(task->pointsAdded + task->pointsTotal);
  } else {
    task->portraitsA = 0;
    for (i = 0; i < data_022a7a88.countA; i++) {
      /* Needed to match: the TRUE/FALSE test keeps the movne/moveq pair. */
      if ((GAME->players[data_022a7a88.sideA[i]].back.id ? TRUE : FALSE) !=
          FALSE) {
        task->portraitsA |= 1 << i;
      }
    }
    Task_JumpToLabel((Task*)task, 1);
  }
}

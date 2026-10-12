#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_02160ac0.h"
#include "game/play_records.h"
#include "game/task.h"

void func_02028a30(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void NumberFont_LoadLow(void);
u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
void Mem_Free(void* block);
int CombatCo_IdFromNumber(int value); /* row CO to CO id */
void Co_LoadGraphicsAndPalette(int co, int tile, int row);
void func_02039f80(int firstTile, int palette);
void Menu_Reset(void);
void Wallpaper_LoadBothOnLayer3(int co);
void GameFlag_Set(int id, int value);
void ResultPoint_Init(int firstTile, int paletteSlot);
void ResultPoint_LoadPart(int part);
BOOL ScoreList_AddRow(int kind);
void func_0203b194(int value);
void ScoreList_FitFirstPage(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const char gResultLionResPath[]; /* "/ohashi/res_result_lion" */
extern const TaskScriptCmd data_0216168c[];

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Fade frame counter; CombatResult_FadeIn counts it up to 32. */
#define FADE_FRAME(task) (*(s16*)&(task)->unk_4c)

/* Nonzero: the score list gets the bonus rows 21 and 13-16. */
#define SHOW_BONUS_ROWS(task) ((task)->unk_52)

/* Brightens every BG layer and the sprites of the current screen to white. */
#define SET_ALL_WHITE()                                      \
  data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;            \
  data_027c0280->blendEva = 0;                               \
  data_027c0280->blendEvb = 0;                               \
  data_027c0280->blendEvy = 16;                              \
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;    \
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;    \
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;    \
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;    \
  data_027c0280->blendTopObj = 1;                            \
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0; \
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0; \
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0; \
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0; \
  data_027c0280->blendBottomObj = 0

/* Sets up the Combat mode result screen. Both screens start white for
 * the fade-in; the second screen gets the /ohashi/res_result_lion
 * background, the main screen the CO of data_02160abc->unk_0b. When
 * data_02160abc->unk_0a is 1 (probably: row 1 won), the stage in
 * data_02160ac0->unk_7d advances; after the last stage (5) it goes back to
 * 0, a clear is counted in gPlayRecords, game flag 42 is set and the bonus
 * rows are shown. unk_7c nonzero always shows them. Then it fills the
 * score list and starts the screen's child task. */
void CombatResult_Init(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  int rowsAdded;

  func_02028a30();

  SWITCH_TO(MAIN_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  SWITCH_BACK();

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(MAIN_MAP);
  SET_ALL_WHITE();
  Screen_SetCurrentBoth(SECOND_MAP);
  SET_ALL_WHITE();
  SWITCH_BACK();

  NumberFont_LoadLow();
  file = File_Load(gResultLionResPath);
  SWITCH_TO(SECOND_MAP);
  Data_Unpack(
      file, data_027c0280->bgVram +
                (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14));
  Data_Unpack(file + 0x22ec, data_027c0280->layer0Map);
  Data_Unpack(file + 0x2568, data_027c0280->layer1Map);
  Palette_Load(file + 0x27a4, 0, 0x40);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 4;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 4;
  SWITCH_BACK();
  Mem_Free(file);

  SWITCH_TO(MAIN_MAP);
  Co_LoadGraphicsAndPalette(CombatCo_IdFromNumber(data_02160abc->unk_0b), 0, 0);
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  func_02039f80(0, 0);
  SWITCH_BACK();

  Menu_Reset();
  Wallpaper_LoadBothOnLayer3((u8)CombatCo_IdFromNumber(data_02160abc->unk_0b));
  FADE_FRAME(task) = 0;

  if (data_02160ac0->unk_7c == 0) {
    if (data_02160abc->unk_0a == 1) {
      if (data_02160ac0->unk_7d == 5) {
        switch (data_02160ac0->unk_84) {
          case 0:
            if (gPlayRecords.combatClears[0] < 999999) {
              gPlayRecords.combatClears[0]++;
            }
            break;
          case 1:
            if (gPlayRecords.combatClears[1] < 999999) {
              gPlayRecords.combatClears[1]++;
            }
            break;
          case 2:
            if (gPlayRecords.combatClears[2] < 999999) {
              gPlayRecords.combatClears[2]++;
            }
            break;
        }
        data_02160ac0->unk_7d = 0;
        GameFlag_Set(42, 1);
        SHOW_BONUS_ROWS(task) = 1;
      } else {
        data_02160ac0->unk_7d++;
        SHOW_BONUS_ROWS(task) = 0;
      }
    }
  } else {
    SHOW_BONUS_ROWS(task) = 1;
  }

  ResultPoint_Init(0x1b0, 1);
  rowsAdded = 0;
  ResultPoint_LoadPart(4);
  ResultPoint_LoadPart(1);
  ResultPoint_LoadPart(2);
  ResultPoint_LoadPart(3);
  if (ScoreList_AddRow(17) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(18) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(19) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(20) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(10) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(11) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(12) == TRUE) {
    rowsAdded++;
  }
  if (SHOW_BONUS_ROWS(task) != 0) {
    ScoreList_AddRow(21);
    ScoreList_AddRow(13);
    ScoreList_AddRow(14);
    ScoreList_AddRow(15);
    ScoreList_AddRow(16);
  }
  func_0203b194(rowsAdded);
  ScoreList_FitFirstPage();
  Task_Create(data_0216168c, task);
  FADE_FRAME(task) = 0;
}

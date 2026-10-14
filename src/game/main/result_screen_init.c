#include "types.h"
#include "game/task.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
u8* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
void Palette_Flush(void);
void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* block);
BOOL CoExp_IsSpecialMission(void);
u8* ResultObj_Load(int firstTile, int palette);
void ResultPoint_Init(int firstTile, int paletteSlot);
void ResultPoint_LoadPart(int part);
BOOL ScoreList_AddRow(int kind);
void func_0203b194(int value);
void ScoreList_FitFirstPage(void);
void Screens_SetBg3Zoom(int scale, int angle);
void NumberFont_LoadHigh(void);
int Player_GetCurrentOrFirstInGame(void);
void Wallpaper_LoadBothOnLayer2(u32 index);
void Wallpaper_LoadOnLayer2(u32 index);
void Menu_Reset(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern const u8
    data_02159ee0[]; /* main screen layer config for Screen_InitLayers */
extern const u8 data_02159f30[];   /* sub screen layer config */
extern const char data_0215a3bc[]; /* "/ohashi/res_result_lion" */
extern const char data_0215a3d4[]; /* "/ohashi/res_result" */
extern const char data_0215a3e8[]; /* "/ohashi/res_result_lang" */

#define MAIN_MAP (data_027c0288.unk_00)
#define SUB_MAP (data_027c0288.unk_04)
#define MAIN_GFX ((Unk_027c0280*)data_027c0288.unk_00)

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

#define CHAR_BASE(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* The result screen task keeps a 16-bit counter in unk_4c (CombatResult_Init: fade frames). */
#define TASK_COUNTER(task) (*(s16*)&(task)->unk_4c)

/* Sets up the result screen after a battle. Both screens go to BG mode 1, start
 * fully brightened to white for the fade-in, and get their layers from
 * res_result_lion (sub screen BG0 and BG1), res_result (main screen BG3
 * and BG1, copied to the sub screen's BG3) and the result sprites. Then it
 * fills the score list with rows 1-9, zooms BG3 and loads the CO
 * wallpapers of the current player. */
void ResultScreen_Init(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  int i;
  int rowsAdded; /* needed to match: later also holds the CO id for the wallpapers */

  SWITCH_TO(MAIN_MAP);
  Screen_SetBgMode(1);
  Screen_InitLayers(data_02159ee0);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 3;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 0;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 64;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  /* BG3 is a rotation/scaling BG in mode 1: bit 13 makes it wrap around. */
  data_027c0280->bgCnt[data_027c0280->layer3Bg].extPltt = 1;
  SWITCH_BACK();

  SWITCH_TO(SUB_MAP);
  Screen_SetBgMode(1);
  Screen_InitLayers(data_02159f30);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 3;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 0;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 16;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 4;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 256;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 4;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 64;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].extPltt = 1;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  SWITCH_BACK();

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(MAIN_MAP);
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrentBoth(SUB_MAP);
  data_027c0280->masterBrightness = 0;
  SWITCH_BACK();

  file = File_Load(data_0215a3bc);
  SWITCH_TO(SUB_MAP);
  Data_Unpack(file, CHAR_BASE(data_027c0280->layer0Bg));
  Data_Unpack(file + 0x22ec, data_027c0280->layer0Map);
  Data_Unpack(file + 0x2568, data_027c0280->layer1Map);
  Palette_Load(file + 0x27a4, 0x100, 0x40);
  /* both maps use BG palette 8 */
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x8000;
    data_027c0280->layer1Map[i] += 0x8000;
  }
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  SWITCH_BACK();
  Mem_Free(file);

  file = File_Load(data_0215a3d4);
  SWITCH_TO(MAIN_MAP);
  if (CoExp_IsSpecialMission() == TRUE) {
    Data_Unpack(file + 0x25d4, CHAR_BASE(data_027c0280->layer3Bg));
    Data_Unpack(file + 0x4368, data_027c0280->layer3Map);
    Palette_Load(file + 0x46c0, 0, 0x100);
  } else {
    Data_Unpack(file, CHAR_BASE(data_027c0280->layer3Bg));
    Data_Unpack(file + 0x21b4, data_027c0280->layer3Map);
    Palette_Load(file + 0x24d4, 0, 0x100);
  }
  Gfx_RequestLayerCopy(3);
  Data_Unpack(file + 0x47c0, CHAR_BASE(data_027c0280->layer1Bg));
  Data_Unpack(file + 0x4fb4, data_027c0280->layer1Map + 0x400);
  Palette_Load(file + 0x5170, 0x1c0, 0x20);
  /* second half of the layer 1 map: BG palette 14 */
  for (i = BG_SCREEN_ENTRIES; i < BG_SCREEN_ENTRIES + BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0xe000;
  }
  Gfx_RequestLayerCopy(1);
  Data_Unpack(file + 0x5190, data_027c0280->objVram + 0x3b80);
  Data_Unpack(file + 0x57f0, data_027c0280->objVram + 0x5940);
  Data_Unpack(file + 0x5940, data_027c0280->objVram + 0x3900);
  Palette_Load(file + 0x59b8, 0x200, 0x20);
  SWITCH_BACK();

  SWITCH_TO(SUB_MAP);
  if (CoExp_IsSpecialMission() == TRUE) {
    Mem_Copy32(MAIN_GFX->unk_120, CHAR_BASE(data_027c0280->layer3Bg), 0x4000);
    Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map,
               GFX_LAYER_MAP_SIZE);
    Palette_Load(file + 0x46c0, 0, 0x100);
  } else {
    Mem_Copy32(MAIN_GFX->unk_120, CHAR_BASE(data_027c0280->layer3Bg), 0x4000);
    Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map,
               GFX_LAYER_MAP_SIZE);
    Palette_Load(file + 0x24d4, 0, 0x100);
  }
  Gfx_RequestLayerCopy(3);
  Mem_Copy32(MAIN_GFX->objVram + 0x3b80, data_027c0280->objVram + 0x3b80,
             0x1dc0);
  Mem_Copy32(MAIN_GFX->objVram + 0x5940, data_027c0280->objVram + 0x5940,
             0x800);
  Mem_Copy32(MAIN_GFX->objVram + 0x3900, data_027c0280->objVram + 0x3900,
             0x280);
  Palette_Load(file + 0x59b8, 0x200, 0x20);
  SWITCH_BACK();

  Mem_Copy32(file + 0x59d8, RESULT_WORK->unk_000, 0x16a);
  Mem_Copy32(file + 0x5b44, RESULT_WORK->unk_16c, 0xaa);
  Mem_Copy32(file + 0x5bf0, RESULT_WORK->unk_218, 0x4a);
  Mem_Free(file);

  file = File_Load(File_LocalizePath(data_0215a3e8));
  Data_Unpack(file, RESULT_WORK->unk_264);
  Mem_Free(file);

  file = ResultObj_Load(0, 1);
  SWITCH_TO(MAIN_MAP);
  Data_Unpack(file, data_027c0280->objVram + 0x6140);
  Data_Unpack(file + 0x56c, data_027c0280->objVram + 0x6940);
  Data_Unpack(file + 0x9e4, data_027c0280->objVram + 0x7140);
  Data_Unpack(file + 0xd54, data_027c0280->objVram + 0x7940);
  Data_Unpack(file + 0xfb4, data_027c0280->objVram + 0x8140);
  Palette_Load(file + 0x152c, 0x280, 0x80);
  SWITCH_BACK();

  SWITCH_TO(SUB_MAP);
  Data_Unpack(file + 0x13c4, data_027c0280->objVram + 0x6140);
  Mem_Copy32(MAIN_GFX->objVram, data_027c0280->objVram, 0x2180);
  Mem_Copy32(MAIN_GFX->paletteBuffer + 0x220,
             ((Unk_027c0280*)SUB_MAP)->paletteBuffer + 0x220, 0x20);
  Palette_Flush();
  SWITCH_BACK();
  Mem_Free(file);

  ResultPoint_Init(0x10c, 1);
  ResultPoint_LoadPart(0);
  ResultPoint_LoadPart(2);
  ResultPoint_LoadPart(3);
  rowsAdded = 0;
  if (ScoreList_AddRow(1) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(2) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(3) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(4) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(5) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(6) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(7) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(8) == TRUE) {
    rowsAdded++;
  }
  if (ScoreList_AddRow(9) == TRUE) {
    rowsAdded++;
  }
  func_0203b194(rowsAdded);
  ScoreList_FitFirstPage();
  Screens_SetBg3Zoom(0x100, 0x80);
  NumberFont_LoadHigh();

  /* no tag partner: the front CO's wallpaper on both screens; else the
     * front CO on the main screen and the partner on the sub screen */
  if ((data_022a7aba[Player_GetCurrentOrFirstInGame()].unk_00 &
       PLAYER_CO_ID_MASK) == 0) {
    Wallpaper_LoadBothOnLayer2(
        data_022a7ab8[Player_GetCurrentOrFirstInGame()].unk_00 &
        PLAYER_CO_ID_MASK);
  } else {
    rowsAdded = data_022a7ab8[Player_GetCurrentOrFirstInGame()].unk_00 &
                PLAYER_CO_ID_MASK;
    SWITCH_TO(MAIN_MAP);
    Wallpaper_LoadOnLayer2(rowsAdded);
    SWITCH_BACK();
    rowsAdded = data_022a7aba[Player_GetCurrentOrFirstInGame()].unk_00 &
                PLAYER_CO_ID_MASK;
    SWITCH_TO(SUB_MAP);
    Wallpaper_LoadOnLayer2(rowsAdded);
    SWITCH_BACK();
  }

  Menu_Reset();
  TASK_COUNTER(task) = 0;
}

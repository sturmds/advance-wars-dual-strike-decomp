#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Gfx_RequestLayerCopy(int layer);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int baseTile);
void ModeFont_Load(int a, int b);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
const char* Text_Get(u32 textId);
void func_02028a30(void);
void Wallpaper_LoadSelected(void);
void Ov005SoundRoom_StartMusicTask(void);
void func_ov005_02364428(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_StartPictureTask(Unk_ov005_0236bbac* work);
void func_ov005_02364a2c(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_LoadIcons(Unk_ov005_0236bbac* work);
void Ov005_SoundRoom_AddCellButtons(Unk_ov005_0236bbac* work);
void func_ov005_02365100(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_LoadSprites(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_PickCellCo(Unk_ov005_0236bbac* work);
void func_ov005_02365e90(Unk_ov005_0236bbac* work);

extern u8 data_ov005_0236b0bc[];
extern char data_ov005_0236b324[];
extern char data_ov005_0236b330[];
extern const MenuButtonTemplate data_ov005_0236b02c;
extern const MenuButtonTemplate data_ov005_0236af3c;
extern const MenuButtonTemplate data_ov005_0236af64;
extern const MenuButtonTemplate data_ov005_0236af8c;
extern const MenuButtonTemplate data_ov005_0236afb4;

/* Setup step of the Sound Room. Sub screen: all four BGs and OBJ on, no
 * blending, text 0x7d000003 in the mode font. Main screen: layer 1 tiles
 * (Gfx_LoadBmap09f) and tile map (a file), and touch buttons 242-246.
 * Then loads the digit font on both screens, starts on page 0 with no CO
 * picture, and builds the list (the func_ov005_* calls). */
void Ov005_SetUpSoundRoom(Ov005Task* task) {
  Unk_ov005_0236bbac* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  func_02028a30();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_InitLayers(data_ov005_0236b0bc);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Wallpaper_LoadSelected();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ModeFont_Load(0, 0);
  ModeFont_ShowTextOnScreen(Text_Get(0x7d000003), 0, 0, 0x400, 10, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  FS_ChangeDir(data_ov005_0236b324);
  File_UnpackTo(data_027c0280->layer1Map, data_ov005_0236b330);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov005_0236b02c, 242);
  MenuButton_Add(0, 0, &data_ov005_0236af3c, 243);
  MenuButton_Add(0, 0, &data_ov005_0236af64, 244);
  MenuButton_Add(0, 0, &data_ov005_0236af8c, 245);
  MenuButton_Add(0, 0, &data_ov005_0236afb4, 246);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  work->objScale1 = -1;
  work->objScale2 = -1;
  work->objScaleBoth = -1;
  Ov005SoundRoom_LoadSprites(work);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Font_LoadDigits(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Font_LoadDigits(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  work->page = 0;
  work->selection = 0;
  work->shownSelection = 0;
  work->pageStartSelection = 0;
  work->cursorWrap = 0;
  work->displayedRow = 0;
  work->displayedPage = 0;
  func_ov005_02365e90(work);
  work->coId = -1;
  work->loadedCoId = -1;
  Ov005SoundRoom_StartPictureTask(work);
  Ov005SoundRoom_PickCellCo(work);
  func_ov005_02365100(work);
  Ov005_SoundRoom_AddCellButtons(work);
  Ov005SoundRoom_LoadIcons(work);
  func_ov005_02364428(work);
  func_ov005_02364a2c(work);
  Ov005SoundRoom_StartMusicTask();
  task->step = 0;
}

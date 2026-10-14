#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/unit_show_task.h"
#include "game/Unk_ov005_0236bba4.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void func_02028a30(void);
void Ov005_InitStateWork(Unk_ov005_0236bba4* work);
void Ov005_RunState(Unk_ov005_0236bba4* work);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
void TpIcon3_LoadTo(u8* dest);
void ModeFont_LoadHere(int tile, int paletteSlot);
const void* Text_Get(u32 textId);
/* Needed to match: this call passes a seventh argument that the function
 * does not read. */
void ModeFont_StartTextTask(const void* text, int tile, int paletteSlot,
                            int unk_56, u8 unk_5c, int unk_58, int unused);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void* File_Load(const char* path);
ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void func_ov005_0235ec40(Unk_ov005_0236bba4* work);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const MenuButtonTemplate data_ov005_0236a634;
extern const MenuButtonTemplate data_ov005_0236a684;
extern const MenuButtonTemplate data_ov005_0236a6ac;
extern const MenuButtonTemplate data_ov005_0236a6d4;
extern char data_ov005_0236aa80[]; /* "/rikiishi" */
extern char data_ov005_0236aa8c[]; /* "04f" */
extern char data_ov005_0236aa90[]; /* "051" */
extern char data_ov005_0236aa94[]; /* "050" */

/* First step of the state screen (work block Unk_ov005_0236bba4). Both
 * screens: only BG3 shown (the main screen also OBJ), layer priorities 0 to
 * 3, darken blend on layers 2 and 3 with no fade yet, no scroll. Starts the
 * state table at state 0, then on the main screen adds buttons 241-244,
 * starts the title text and loads /rikiishi/04f, 051 and 050, making three
 * sprites from 050. */
void Ov005_SetUpStateScreen(Ov005Task* task) {
  Unk_ov005_0236bba4* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280*
      loadSaved0280; /* needed to match: its own variable for the file block */
  int i;

  work = task->work;
  func_02028a30();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt &= ~0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 0;
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

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 0;
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

  work->state = 0;
  Ov005_InitStateWork(work);
  Ov005_RunState(work);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x5400);
  TpIcon3_LoadTo(data_027c0280->objVram + 0x4800);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov005_0236a684, 243);
  MenuButton_Add(0, 0, &data_ov005_0236a634, 244);
  MenuButton_Add(0, 0, &data_ov005_0236a6ac, 241);
  MenuButton_Add(0, 0, &data_ov005_0236a6d4, 242);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  ModeFont_LoadHere(0x80, 2);
  ModeFont_StartTextTask(Text_Get(0x7d00001b), 0x80, 2, 0, 0, 0, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  loadSaved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_ov005_0236aa80);
  File_UnpackTo(data_027c0280->objVram + 0x4000, data_ov005_0236aa8c);
  Palette_LoadFromFile(data_ov005_0236aa90, 0, 0x340, 32);
  work->spriteFile = File_Load(data_ov005_0236aa94);
  for (i = 0; i < (int)ARRAY_COUNT(work->unk_390); i++) {
    work->unk_390[i] = SpriteAnim_Create(work->spriteFile, 0, 2);
    ((ShowSprite*)work->unk_390[i])->unk_24 = 0xa200;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(loadSaved0280);

  func_ov005_0235ec40(work);
  work->scrollX = 0;
  work->scrollY = 0;
  work->scrollStepX = 0;
  work->unk_28 = 0;
  work->scrollMode = 0;
  task->step = 0;
}

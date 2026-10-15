#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba0.h"
#include "game/screen_other_map.h"
#include "game/icon.h"
#include "sdk/fs.h"

void func_02028a30(void);
void File_UnpackTo(void* dest, const char* fileName);
void Gfx_RequestLayerCopy(int layer);
void Font_LoadDigits(int baseTile);
void Gfx_LoadBmap09f(void* dst, int unused, int paletteSlot);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
/* Defined with no parameters; this call passes the task in r0 anyway. */
void Ov005_AddStateScreenButtons(Ov005Task* task);
int Ov005_FindSavedCoListEntry(void);
u8 Ov005_GetCoListEntryRow(int entry);
void Ov005_AddCoListButtons(Ov005Task* task);
void Wallpaper_LoadBothOnLayer3(int co);
void ModeFont_Load(int tile, int paletteSlot);
const void* Text_Get(u32 textId);
void ModeFont_ShowText(const void* text, int tile, int paletteSlot, int a,
                       int b);
void HelpBand_Start(int tile, int a);
void HelpText_Show(u32 textId, int tile);
void IconBanks_ClearSlots(void);

extern char data_ov005_0236a624[]; /* "/rikiishi" */
extern char data_ov005_0236a630[]; /* "039" */
extern u8 data_ov005_0236a48c[];   /* CO id of each list entry */

/* First step of the CO list screen (work block Unk_ov005_0236bba0). Both
 * screens: BG0, BG3 and OBJ on (the main screen also BG1), layer priorities
 * 0 to 3, darken blend on layer 3 with no fade yet, no scroll. Loads the main
 * screen's layer 1 map from /rikiishi/039, the digit font and the buttons,
 * puts the cursor on the entry of the wallpaper saved in gPlayRecords, loads
 * that wallpaper with Wallpaper_LoadBothOnLayer3, and writes the sub screen's
 * title and help text. */
void Ov005_SetUpCoListScreen(Ov005Task* task) {
  Unk_ov005_0236bba0* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  work = task->work;
  func_02028a30();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->dispCnt |= 0x100;
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
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
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
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
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
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
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
  FS_ChangeDir(data_ov005_0236a624);
  File_UnpackTo(data_027c0280->layer1Map, data_ov005_0236a630);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Font_LoadDigits(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x2000);
  Ov005_AddStateScreenButtons(task);
  work->unk_04 = 0;
  work->unk_08 = 0;
  work->unk_0c = 0;
  work->unk_10 = 0;
  work->selectedEntry = Ov005_FindSavedCoListEntry();
  work->cursorRow = Ov005_GetCoListEntryRow(work->selectedEntry);
  work->unk_840 = work->selectedEntry;
  Ov005_AddCoListButtons(task);
  work->unk_844 = work->selectedEntry;
  work->unk_848 = work->unk_844;
  work->unk_84c = 1;
  Wallpaper_LoadBothOnLayer3(data_ov005_0236a48c[work->selectedEntry]);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ModeFont_Load(0, 0);
  ModeFont_ShowText(Text_Get(0x7d000011), 0, 0, 0, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  HelpBand_Start(0x3f8, 1);
  HelpText_Show(0x80000030, 0x40);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  work->scrollX = 0;
  work->scrollY = 0;
  work->unk_82c = 0;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = work->scrollX >> 12;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  IconBanks_ClearSlots();
  IconBank_Init(4, data_027c0280->objVram, 0x200, 24);
  Icon_Load(0xd6);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->step = 0;
}

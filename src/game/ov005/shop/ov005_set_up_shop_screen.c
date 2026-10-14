#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "game/icon.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbb0.h"

void func_02028a30(void);
void Wallpaper_LoadSelected(void);
void IconBanks_ClearSlots(void);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
void TpIcon3_LoadTo(u8* dest);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void ModeFont_Load(int x, int y);
const void* Text_Get(u32 textId);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void Co_LoadGraphics(int coId, int firstTile);
void Co_LoadPalette(int coId, int paletteSlot);
void Gfx_LoadBmap09f(void* target, int a, int b);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Gfx_RequestLayerCopy(int layer);
void Font_LoadDigits(int baseTile);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Lighten(u8* palette, int a, int b, int c);
void Palette_Flush(void);
void Ov005_LoadShopItems(Unk_ov005_0236bbb0* work);
void Ov005_InitShopPaging(Unk_ov005_0236bbb0* work);
struct Task* Sound_StartSongSwitch(
    int song, int arg); /* task: fade out the song, then start `song` */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_ov005_0236b4e0[]; /* "/rikiishi" */
extern char data_ov005_0236b4ec[]; /* "044" */

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Task step: sets up Hachi's shop. On data_027c0288.unk_04: the title text
 * (0x7d000001) and Hachi's pictures. On data_027c0288.unk_00: the
 * background from /rikiishi, the digit font, BG palette 0 copied to
 * palette 9, the icons of bank 4 (212 and the digits 140-149 that
 * Ov005_DrawShopPoints draws the points with), then the list of items for
 * sale (Ov005_LoadShopItems) and its first page. Starts song 12 and resets
 * task->step to 0 for the next update function. */
void Ov005_SetUpShopScreen(Ov005Task* task) {
  Unk_ov005_0236bbb0* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  func_02028a30();
  Wallpaper_LoadSelected();

  SWITCH_TO(data_027c0288.unk_04);
  ModeFont_Load(0, 0);
  ModeFont_ShowTextOnScreen(Text_Get(0x7d000001), 0, 0, 0x800, 10, 0);
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_04);
  Co_LoadGraphics(CO_HACHI, 0x200);
  Co_LoadPalette(CO_HACHI, 26);
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  FS_ChangeDir(data_ov005_0236b4e0);
  File_UnpackTo(data_027c0280->layer1Map, data_ov005_0236b4ec);
  Gfx_RequestLayerCopy(1);
  Font_LoadDigits(0);
  Mem_Copy32(data_027c0280->paletteBuffer, data_027c0280->paletteBuffer + 0x120,
             PALETTE_16_SIZE);
  Palette_Lighten(data_027c0280->paletteBuffer, 9, 1, 12);
  Palette_Flush();
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_00);
  IconBanks_ClearSlots();
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_00);
  IconBank_Init(4, data_027c0280->objVram, 0x100, 26);
  Icon_Load(212);
  Icon_Load(140);
  Icon_Load(141);
  Icon_Load(142);
  Icon_Load(143);
  Icon_Load(144);
  Icon_Load(145);
  Icon_Load(146);
  Icon_Load(147);
  Icon_Load(148);
  Icon_Load(149);
  Icon_Load(150);
  Icon_Load(151);
  Icon_Load(152);
  Icon_Load(153);
  Icon_Load(154);
  Icon_Load(155);
  Icon_Load(156);
  Icon_Load(157);
  Icon_Load(158);
  Icon_Load(159);
  Icon_Load(162);
  SWITCH_BACK();

  SWITCH_TO(data_027c0288.unk_00);
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x4000);
  TpIcon3_LoadTo(data_027c0280->objVram + 0x4800);
  SWITCH_BACK();

  work->boughtSomething = 0;
  work->shopItemCount = 0x100;
  work->highlightCursorRow = 0;
  work->showCursor = 0;
  Ov005_LoadShopItems(work);
  Ov005_InitShopPaging(work);
  work->buyRequested = 0;
  work->leaveRequested = 0;
  work->prevPageRequested = 0;
  work->nextPageRequested = 0;
  work->transition = 0;
  work->redrawRows = 1;
  work->songTask = Sound_StartSongSwitch(12, 0);
  task->step = 0;
}

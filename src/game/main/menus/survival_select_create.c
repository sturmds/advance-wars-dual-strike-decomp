#include "types.h"
#include "game/page_strip.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

s32 HwDiv32(s32 num, s32 den);
void* Mem_Alloc(u32 size);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void* text, int tile, int paletteSlot, int a,
                       int b);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetCurrentGfx(void* state);
int Survival_GetMapCount(int mode);
const void* Text_Get(u32 textId);
void Wallpaper_LoadSelected(void);
void func_02028a30(void);

extern Unk_022a7b80 data_022a7b80;
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Allocates the page strip work area of the Survival map select. List 0
 * holds the maps of mode variant n (0-2) and list 1 those of n + 3, cut to
 * the saved record value of n + 3; unk_03 gets the page count. Then turns
 * on DISPCNT bit 6 on both screens, loads the title text of the mode and the
 * wallpaper. */
void SurvivalSelect_Create(void) {
  Unk_027c0280* savedState;
  Unk_02080440* saved027c;

  data_0229c744 = Mem_Alloc(sizeof(PageStrip));
  data_0229c744->lists[0].cursor = 0;
  data_0229c744->lists[1].cursor = 0;
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      data_0229c744->lists[0].count = Survival_GetMapCount(0);
      data_0229c744->lists[1].count = Survival_GetMapCount(3);
      data_0229c744->lists[1].unk_02 = gPlayRecords.survivalRecords[3].value;
      break;
    case 2:
    case 5:
      data_0229c744->lists[0].count = Survival_GetMapCount(2);
      data_0229c744->lists[1].count = Survival_GetMapCount(5);
      data_0229c744->lists[1].unk_02 = gPlayRecords.survivalRecords[5].value;
      break;
    default:
      data_0229c744->lists[0].count = Survival_GetMapCount(1);
      data_0229c744->lists[1].count = Survival_GetMapCount(4);
      data_0229c744->lists[1].unk_02 = gPlayRecords.survivalRecords[4].value;
      break;
  }
  data_0229c744->lists[0].unk_02 = data_0229c744->lists[0].count;
  data_0229c744->lists[0].unk_03 = 1;
  if (data_0229c744->lists[1].count > data_0229c744->lists[1].unk_02) {
    data_0229c744->lists[1].count = data_0229c744->lists[1].unk_02;
    data_0229c744->lists[1].unk_03 = 1;
  } else {
    data_0229c744->lists[1].unk_03 = HwDiv32(data_0229c744->lists[1].unk_02 - 1,
                                             data_0229c744->lists[1].count) +
                                     1;
  }
  func_02028a30();

  savedState = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedState);

  ModeFont_Load(0x400, 0);
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      ModeFont_ShowText(Text_Get(0x7d00000c), 0x400, 0, 0, 0);
      break;
    case 2:
    case 5:
      ModeFont_ShowText(Text_Get(0x7d00000d), 0x400, 0, 0, 0);
      break;
    default:
      ModeFont_ShowText(Text_Get(0x7d00000e), 0x400, 0, 0, 0);
      break;
  }
  Wallpaper_LoadSelected();
}

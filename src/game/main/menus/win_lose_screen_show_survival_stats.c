#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7b80.h"
#include "game/player.h"
#include "game/win_lose.h"
#include "game/math.h"

void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const void* text,
                               u16 unk_30, int arg);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
void Layer0_PutNumber(int x, int y, u32 value);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void Gfx_RequestLayerCopy(int layer);
const char* Map_GetCurrentName(void);
void* func_0203b614(void);
void Task_JumpToLabel(WinLoseTask* task, int label);

extern Unk_022a7b80 data_022a7b80;

/* In game mode 3 (likely Survival) prints on layer 0 the string from
 * Map_GetCurrentName, then two rows by the mode variant in data_022a7b80.mode:
 * 0 or 3 time as m:ss (player 1's timer, then the limit), 2 or 5 days
 * (the current day, then the limit), others funds (player 1's unk_04,
 * then the limit). In modes 2 and 3 it adds the button from
 * func_0203b614 and jumps to label 0; mode 3 also sets frame to 12. */
void WinLoseScreen_ShowSurvivalStats(WinLoseTask* task) {
  u16* map;
  u8 mode;

  if (MAP_STATE->gameMode == 3) {
    Gfx_RequestLayerCopy(0);
    map = data_027c0280->layer0Map; /* needed to match: read before the call */
    Text_PrintStringImmediate(3, 15, map, Map_GetCurrentName(), 0, 0);
    if (data_022a7b80.mode == 0 || data_022a7b80.mode == 3) {
      Text_PrintImmediate(14, 15, data_027c0280->layer0Map, 0x79000006, 0, 0);
      Layer0_PutRatio(
          26, 15, HwDiv32(MAP_STATE->players[1].playTimeFrames, 3600),
          HwMod32(HwDiv32(MAP_STATE->players[1].playTimeFrames, 60), 60));
      Text_PrintImmediate(14, 17, data_027c0280->layer0Map, 0x79000007, 0, 0);
      Layer0_PutRatio(26, 17, HwDiv32(data_022a7b80.budgetLeft, 3600),
                      HwMod32(HwDiv32(data_022a7b80.budgetLeft, 60), 60));
    } else if (data_022a7b80.mode == 2 || data_022a7b80.mode == 5) {
      Text_PrintImmediate(14, 15, data_027c0280->layer0Map, 0x79000009, 0, 0);
      Layer0_PutNumber(26, 15, MAP_STATE->day);
      Text_PrintImmediate(14, 17, data_027c0280->layer0Map, 0x7900000a, 0, 0);
      Layer0_PutNumber(26, 17, data_022a7b80.budgetLeft);
    } else {
      Text_PrintImmediate(14, 15, data_027c0280->layer0Map, 0x7900000c, 0, 0);
      Layer0_PutNumberThenText4(26, 15, MAP_STATE->players[1].unk_04);
      Text_PrintImmediate(14, 17, data_027c0280->layer0Map, 0x7900000d, 0, 0);
      Layer0_PutNumberThenText4(26, 17, data_022a7b80.budgetLeft);
    }
  }
  mode = MAP_STATE->gameMode;
  if (mode == 2 || mode == 3) {
    if (mode == 3) {
      task->frame = 12;
    }
    func_0203b614();
    Task_JumpToLabel(task, 0);
  }
}

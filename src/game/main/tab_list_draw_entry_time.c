#include "types.h"
#include "game/tab_list.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

u32 HwDivU32(u32 numerator, u32 denominator);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
u32 HwModU32(u32 numerator, u32 denominator);
void Layer0_PutTime(int x, int y, u32 hours, u32 minutes, u32 seconds);
extern Unk_027c0280* data_027c0280;

/* TabListEntry.draw of the entries whose number is a time: the text at x,
 * then the value (frames, 60 to the second) as hours, minutes and seconds
 * ending at x + 20. */
void TabList_DrawEntryTime(int x, int y, int row, int col) {
  u32 frames;
  u32 seconds;
  u32 minutes;
  u32 hours;

  frames = data_0215d6c4[row][col].getValue(row, col);
  seconds = HwMod32(HwDivU32(frames, 60), 60);
  minutes = HwDivU32(frames, 3600);
  hours = HwDiv32(minutes, 60);
  minutes = HwModU32(minutes, 60);
  Text_PrintId(x, y, data_027c0280->layer0Map, data_0215d6c4[row][col].textId,
               0, ((y - 5) >> 1) * 64 + 128);
  Layer0_PutTime(x + 20, y, hours, minutes, seconds);
}

#include "types.h"
#include "game/play_records.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
void func_02026ddc(int x, int y, u32 value);
int SurvivalRecord_Exists(int mode);
int Survival_GetModeBudget(int mode);

/* Survival mode variant 3: on layer 0 from column x, prints the labels, the
 * mode's time from Survival_GetModeBudget (seconds, shown as h:mm) on row y and, on
 * row y + 2, the saved record's value, or text 0xc00000af when no record is
 * saved. */
void SurvivalSelect_DrawMode3Record(int x, int y) {
  Text_PrintImmediate(x, y, data_027c0280->layer0Map, 0x79000002, 0, 0);
  Text_PrintImmediate(x + 11, y, data_027c0280->layer0Map, 0x79000005, 0, 0);
  Text_PrintImmediate(x + 11, y + 2, data_027c0280->layer0Map, 0x7900000e, 0,
                      0);
  Layer0_PutRatio(x + 23, y, HwDiv32(Survival_GetModeBudget(3), 3600),
                  HwMod32(HwDiv32(Survival_GetModeBudget(3), 60), 60));
  if (SurvivalRecord_Exists(3) == 1) {
    func_02026ddc(x + 23, y + 2, gPlayRecords.survivalRecords[3].value);
  } else {
    Text_PrintImmediate(x + 22, y + 2, data_027c0280->layer0Map, 0xc00000af, 0,
                        0);
  }
}

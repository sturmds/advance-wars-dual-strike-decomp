#include "types.h"
#include "game/play_records.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void func_02026ddc(int x, int y, u32 value);
int SurvivalRecord_Exists(int mode);
int Survival_GetModeBudget(int mode);

/* Survival mode variant 4: on layer 0 from column x, prints the labels, the
 * mode's value from Survival_GetModeBudget on row y and, on row y + 2, the saved
 * record's value, or text 0xc00000af when no record is saved. */
void SurvivalSelect_DrawMode4Record(int x, int y) {
  Text_PrintImmediate(x, y, data_027c0280->layer0Map, 0x79000002, 0, 0);
  Text_PrintImmediate(x + 11, y, data_027c0280->layer0Map, 0x7900000b, 0, 0);
  Text_PrintImmediate(x + 11, y + 2, data_027c0280->layer0Map, 0x7900000e, 0,
                      0);
  Layer0_PutNumberThenText4(x + 23, y, Survival_GetModeBudget(4));
  if (SurvivalRecord_Exists(4) == 1) {
    func_02026ddc(x + 23, y + 2, gPlayRecords.survivalRecords[4].value);
  } else {
    Text_PrintImmediate(x + 22, y + 2, data_027c0280->layer0Map, 0xc00000af, 0,
                        0);
  }
}

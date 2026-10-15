#include "types.h"
#include "game/play_records.h"
#include "game/Unk_02088d04.h"

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Layer0_PutNumber(int x, int y, u32 value);
void func_02026ddc(int x, int y, u32 value);
int SurvivalRecord_Exists(int mode);
int Survival_GetModeBudget(int mode);

/* Survival mode variant 5: on layer 0 from column x, prints the labels, the
 * mode's value from Survival_GetModeBudget on row y and, on row y + 2, the saved
 * record's value, or text 0xc00000af when no record is saved. */
void SurvivalSelect_DrawMode5Record(int x, int y) {
  Text_PrintImmediate(x, y, data_027c0280->layer0Map, 0x79000002, 0, 0);
  Text_PrintImmediate(x + 11, y, data_027c0280->layer0Map, 0x79000008, 0, 0);
  Text_PrintImmediate(x + 11, y + 2, data_027c0280->layer0Map, 0x7900000e, 0,
                      0);
  Layer0_PutNumber(x + 23, y, Survival_GetModeBudget(5));
  if (SurvivalRecord_Exists(5) == 1) {
    func_02026ddc(x + 23, y + 2, gPlayRecords.survivalRecords[5].value);
  } else {
    Text_PrintImmediate(x + 22, y + 2, data_027c0280->layer0Map, 0xc00000af, 0,
                        0);
  }
}

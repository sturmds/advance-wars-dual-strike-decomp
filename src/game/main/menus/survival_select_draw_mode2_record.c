#include "types.h"
#include "game/play_records.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Layer0_PutNumber(int x, int y, u32 value);
void Survival_DrawMapCount(int x, int y, u32 value);
int SurvivalRecord_Exists(int mode);
int Survival_GetModeBudget(int mode);
int Survival_GetMapCount(int mode);

/* Survival mode variant 2: on layer 0 from column x, prints the mode's map
 * count (Survival_GetMapCount), the labels, the mode's value from Survival_GetModeBudget on
 * row y and, on row y + 2, what the saved record left of that value, or text
 * 0xc00000af when no record is saved. */
void SurvivalSelect_DrawMode2Record(int x, int y) {
  Survival_DrawMapCount(x, y, Survival_GetMapCount(2));
  Text_PrintImmediate(x + 11, y, data_027c0280->layer0Map, 0x79000008, 0, 0);
  Text_PrintImmediate(x + 11, y + 2, data_027c0280->layer0Map, 0x79000009, 0,
                      0);
  Layer0_PutNumber(x + 23, y, Survival_GetModeBudget(2));
  if (SurvivalRecord_Exists(2) == 1) {
    Layer0_PutNumber(
        x + 23, y + 2,
        Survival_GetModeBudget(2) - gPlayRecords.survivalRecords[2].value);
  } else {
    Text_PrintImmediate(x + 22, y + 2, data_027c0280->layer0Map, 0xc00000af, 0,
                        0);
  }
}

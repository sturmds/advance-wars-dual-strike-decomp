#include "types.h"
#include "game/Unk_022a7b80.h"

void SurvivalSelect_DrawMode0Record(int x, int y);
void SurvivalSelect_DrawMode1Record(int x, int y);
void SurvivalSelect_DrawMode2Record(int x, int y);
void SurvivalSelect_DrawMode3Record(int x, int y);
void SurvivalSelect_DrawMode4Record(int x, int y);
void SurvivalSelect_DrawMode5Record(int x, int y);
int SurvivalMode_HasSecondRow(void);

extern Unk_022a7b80 data_022a7b80;

/* Draws the record panel of the current mode at row 7 and, when
 * SurvivalMode_HasSecondRow reports the second row is on offer, the panel of the
 * paired mode (n + 3) at row 15. */
void SurvivalModeMenu_DrawRecords(void) {
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      SurvivalSelect_DrawMode0Record(1, 7);
      if (SurvivalMode_HasSecondRow() == 1) {
        SurvivalSelect_DrawMode3Record(1, 15);
      }
      break;
    case 2:
    case 5:
      SurvivalSelect_DrawMode2Record(1, 7);
      if (SurvivalMode_HasSecondRow() == 1) {
        SurvivalSelect_DrawMode5Record(1, 15);
      }
      break;
    default:
      SurvivalSelect_DrawMode1Record(1, 7);
      if (SurvivalMode_HasSecondRow() == 1) {
        SurvivalSelect_DrawMode4Record(1, 15);
      }
      break;
  }
}

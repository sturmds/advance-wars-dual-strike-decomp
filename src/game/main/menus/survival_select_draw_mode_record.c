#include "types.h"
#include "game/Unk_022a7b80.h"

void SurvivalSelect_DrawMode0Record(int x, int y);
void SurvivalSelect_DrawMode1Record(int x, int y);
void SurvivalSelect_DrawMode2Record(int x, int y);
void SurvivalSelect_DrawMode3Record(int x, int y);
void SurvivalSelect_DrawMode4Record(int x, int y);
void SurvivalSelect_DrawMode5Record(int x, int y);

extern Unk_022a7b80 data_022a7b80;

/* Draws the bottom screen panel for the current mode (data_022a7b80.mode, 0 to 5) at
 * column 3, row 13. */
void SurvivalSelect_DrawModeRecord(void) {
  switch (data_022a7b80.mode) {
    case 0:
      SurvivalSelect_DrawMode0Record(3, 13);
      break;
    case 3:
      SurvivalSelect_DrawMode3Record(3, 13);
      break;
    case 2:
      SurvivalSelect_DrawMode2Record(3, 13);
      break;
    case 5:
      SurvivalSelect_DrawMode5Record(3, 13);
      break;
    case 1:
      SurvivalSelect_DrawMode1Record(3, 13);
      break;
    default:
      SurvivalSelect_DrawMode4Record(3, 13);
      break;
  }
}

#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* Like Mode_SetVariant, but only returns the value: the mode of the current
 * pair (0/3, 1/4, 2/5) that menu row 0 or row 1 stands for. */
int SurvivalModeMenu_GetRowMode(int row) {
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      if (row == 0) {
        return 0;
      }
      return 3;
    case 2:
    case 5:
      if (row == 0) {
        return 2;
      }
      return 5;
    default:
      if (row == 0) {
        return 1;
      }
      return 4;
  }
}

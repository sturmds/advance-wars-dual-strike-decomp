#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* data_022a7b80.mode holds 0-5. Values n and n + 3 form a pair (0/3, 1/4, 2/5) that
 * differ in the second option, which the menu offers once a game flag is
 * set (SurvivalMode_HasSecondRow). Keeps the pair and picks one of its two values. */
void Mode_SetVariant(int second) {
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      if (second == 0) {
        data_022a7b80.mode = 0;
      } else {
        data_022a7b80.mode = 3;
      }
      break;
    case 2:
    case 5:
      if (second == 0) {
        data_022a7b80.mode = 2;
      } else {
        data_022a7b80.mode = 5;
      }
      break;
    default:
      if (second == 0) {
        data_022a7b80.mode = 1;
      } else {
        data_022a7b80.mode = 4;
      }
      break;
  }
}

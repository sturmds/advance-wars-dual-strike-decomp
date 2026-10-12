#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* Index into the page strip lists: 0 for modes 0 to 2, 1 for modes 3 to 5
 * (the second of each pair). */
int SurvivalSelect_GetList(void) {
  switch (data_022a7b80.mode) {
    case 0:
    case 1:
    case 2:
      return 0;
    default:
      return 1;
  }
}

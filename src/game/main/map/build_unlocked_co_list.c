#include "types.h"
#include "game/co_select_screen.h"
#include "game/unlocks.h"

/* Zero-terminated lists of CO ids. */
extern u8 gDefaultCoList[];

/* Fills data_022a7a68 with the COs of gDefaultCoList, in that order, for
 * which Unlock_IsSet is nonzero (a bit set per CO id, likely "unlocked"). */
void BuildUnlockedCoList(void) {
  int i;
  int count;
  u8* all = gDefaultCoList;

  count = 0;
  i = 0;
  while (all[i] != 0) {
    if (Unlock_IsSet(all[i])) {
      data_022a7a68[count++] = all[i];
    }
    i++;
  }
  data_022a7a68[count] = 0;
}

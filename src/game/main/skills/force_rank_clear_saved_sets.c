#include "types.h"
#include "game/co_data.h"

int ForceRank_SetCount(int mode); /* skill sets per CO: 4 in mode 2, else 1 */
u8* ForceRank_GetSavedSet(int mode, int co,
                          int set); /* a set's saved skill list, or NULL */
void ForceRank_ClearSet(u8* set);   /* zeroes a saved skill list */

/* Clears the saved skill lists of every CO, as ForceRank_GetSavedSet finds
 * them for `mode`. Needed to match: mode is never set, so the original
 * passes whatever the caller left in its register. */
void ForceRank_ClearSavedSets(void) {
  int mode;
  int co;
  int set;
  int setCount = ForceRank_SetCount(0);

  for (co = 0; co < CO_COUNT; co++) {
    for (set = 0; set < setCount; set++) {
      u8* list = ForceRank_GetSavedSet(mode, co, set);

      if (list != NULL) {
        ForceRank_ClearSet(list);
      }
    }
  }
}

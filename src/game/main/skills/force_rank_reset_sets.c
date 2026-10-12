#include "types.h"
#include "game/co_data.h"

int ForceRank_SetCount(int mode); /* skill sets per CO: 4 in mode 2, else 1 */
u8* ForceRank_GetSavedSet(int mode, int co,
                          int set); /* a set's saved skill list, or NULL */
void ForceRank_LoadSets(int mode);
void ForceRank_ClearSet(u8* set); /* zeroes a saved skill list */
void Co_SetForceRankSetIndex(int co, int player, int set);

/* Ends at -1; only its length is used, as the number of save modes. */
extern const int gSaveModeList[];

/* Clears the saved Force Rank lists of every CO in every save mode, makes
 * players 1-4 use set 0-3, and loads the lists of mode 2 into the equipped
 * skills. */
void ForceRank_ResetAllSets(void) {
  int mode;
  int co;
  int set;
  int setCount;

  for (mode = 0; gSaveModeList[mode] != -1; mode++) {
    setCount = ForceRank_SetCount(mode);
    for (co = 0; co < CO_COUNT; co++) {
      for (set = 0; set < setCount; set++) {
        u8* list = ForceRank_GetSavedSet(mode, co, set);

        if (list != NULL) {
          ForceRank_ClearSet(list);
        }
      }
      Co_SetForceRankSetIndex(co, 1, 0);
      Co_SetForceRankSetIndex(co, 2, 1);
      Co_SetForceRankSetIndex(co, 3, 2);
      Co_SetForceRankSetIndex(co, 4, 3);
    }
  }
  ForceRank_LoadSets(2);
}

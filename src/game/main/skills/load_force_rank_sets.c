#include "types.h"
#include "game/co_data.h"

int ForceRank_SetCount(int mode); /* skill sets per CO: 4 in mode 2, else 1 */
u8* ForceRank_GetSavedSet(int mode, int co,
                          int set); /* a set's saved skill list, or NULL */
void Skills_SetFromList(
    const u8* list,
    u32* skills); /* clears skills, then turns on each id in list */

/* Skills each CO has equipped in each set, as Player.skills-style bitfields. */
extern u32 gCoEquippedSkills[CO_COUNT][4][4];

/* Reads every CO's saved skill lists of `mode` back into its equipped skill
 * bitfields; the reverse of ForceRank_SaveSets. */
void ForceRank_LoadSets(int mode) {
  int co;
  int set;
  int setCount;

  setCount = ForceRank_SetCount(mode);
  for (co = 0; co < CO_COUNT; co++) {
    for (set = 0; set < setCount; set++) {
      Skills_SetFromList(ForceRank_GetSavedSet(mode, co, set),
                         gCoEquippedSkills[co][set]);
    }
  }
}

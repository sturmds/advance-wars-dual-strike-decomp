#include "types.h"
#include "game/co_data.h"
#include "game/skills.h"

int ForceRank_SetCount(int mode); /* skill sets per CO: 4 in mode 2, else 1 */
u8* ForceRank_GetSavedSet(int mode, int co,
                          int set); /* a set's saved skill list, or NULL */

/* Skills each CO has equipped in each set, as Player.skills-style bitfields. */

/* Writes every CO's equipped skill bitfields into the saved skill lists of
 * `mode`: up to 4 skill ids per set, ended by 0 when fewer. */
void ForceRank_SaveSets(int mode) {
  u8* saved;
  int co;
  int set;
  int setCount;
  int zero; /* needed to match: both counters start from this local */
  int skill;
  int n;

  setCount = ForceRank_SetCount(mode);
  zero = 0;
  for (co = 0; co < CO_COUNT; co++) {
    for (set = 0; set < setCount; set++) {
      saved = ForceRank_GetSavedSet(mode, co, set);
      if (saved != NULL) {
        n = zero;
        for (skill = n; skill < 128; skill++) {
          if (CheckSkillsEquipped(gCoEquippedSkills[co][set], skill)) {
            saved[n] = skill;
            n++;
          }
        }
        if (n < 4) {
          saved[n] = 0;
        }
      }
    }
  }
}

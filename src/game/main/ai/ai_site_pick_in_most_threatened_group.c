#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"

void func_020937dc(void);
int AiSite_PickMostEnemies(int siteType, int group);

#define GROUP_USED_UP 0xff

/* Takes the group with the highest threat that is not used up and asks
 * AiSite_PickMostEnemies for a site in it. Returns that site when there is
 * one; otherwise marks a group used up and searches again. The group pointer
 * and the best threat are not reset between searches, so the second search
 * starts where the first stopped, and the mark lands on the entry after the
 * last group. Returns -1 when no group is left. */
int AiSite_PickInMostThreatenedGroup(int siteType) {
  AiSiteGroup* group = gAiSiteGroups;
  int bestThreat = 0;
  int i;
  int found;
  int site;

  func_020937dc();
  for (;;) {
    found = -1;
    for (i = 0; i < data_021619c0->slotCount; i++, group++) {
      if (group->unk_01 != GROUP_USED_UP && bestThreat < group->threat) {
        bestThreat = group->threat;
        found = i;
      }
    }
    if (found == -1) {
      return -1;
    }
    site = AiSite_PickMostEnemies(siteType, found);
    if (site != -1) {
      return site;
    }
    group->unk_01 = GROUP_USED_UP;
  }
}

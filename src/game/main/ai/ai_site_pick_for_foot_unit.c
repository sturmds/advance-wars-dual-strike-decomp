#include "types.h"
#include "game/ai_site.h"
#include "game/ai_build_state.h"
#include "game/ai_site_group.h"

void func_020937dc(void);
int AiSite_PickNearestToProperty(int siteType, int group);

#define GROUP_USED_UP 0xff
/* Takes the first group in the list that is not used up and has skipFoot
 * clear, and asks AiSite_PickNearestToProperty for a site in it. Returns that
 * site when there is one; otherwise marks a group used up and searches again.
 * The group pointer is not reset between searches, so the second search
 * starts where the first stopped (as in AiSite_PickInMostThreatenedGroup).
 * Returns -1 when no group is left. */
int AiSite_PickForFootUnit(void) {
  AiSiteGroup* group = gAiSiteGroups;
  int i;
  int found;
  int site;

  func_020937dc();
  for (;;) {
    found = -1;
    for (i = 0; i < data_021619c0->slotCount; i++, group++) {
      if (group->unk_01 != GROUP_USED_UP && !group->skipFoot) {
        found = i;
        break;
      }
    }
    if (found == -1) {
      return -1;
    }
    site = AiSite_PickNearestToProperty(SITE_BASE, found);
    if (site != -1) {
      return site;
    }
    group->unk_01 = GROUP_USED_UP;
  }
}

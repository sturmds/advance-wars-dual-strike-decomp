#include "types.h"
#include "game/ai_build_state.h"

int AiSite_PickInMostThreatenedGroup(int siteType);
int AiSite_PickMostEnemiesAnyGroup(int siteType);
int AiSite_PickNearestEnemyPenalized(int siteType);
int AiSite_PickFirst(int siteType);

/* Picks the site to build at, trying each rule until one finds a site: the
 * most threatened group (only when the map has two or more groups), most
 * enemies in any group, nearest enemy, then the first site. Returns the site
 * index, or -1. */
int AiSite_PickForBuild(int siteType) {
  int site = -1;

  if (data_021619c0->slotCount >= 2) {
    site = AiSite_PickInMostThreatenedGroup(siteType);
  }
  if (site == -1) {
    site = AiSite_PickMostEnemiesAnyGroup(siteType);
  }
  if (site == -1) {
    site = AiSite_PickNearestEnemyPenalized(siteType);
  }
  if (site == -1) {
    site = AiSite_PickFirst(siteType);
  }
  return site;
}

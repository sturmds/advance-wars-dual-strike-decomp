#include "types.h"
#include "game/ai_site.h"

int AiSite_PickForBuild(int siteType);
int AiSite_RateLandmass(int x, int y);

/* Rates the landmass of the base AiSite_PickForBuild picks (see
 * AiSite_RateLandmass for the result), or 0 when there is none. */
int AiSite_RateBaseLandmass(void) {
  int index = AiSite_PickForBuild(SITE_BASE);

  if (index == -1) {
    return 0;
  }
  return AiSite_RateLandmass(data_0229e5bc[index].x, data_0229e5bc[index].y);
}

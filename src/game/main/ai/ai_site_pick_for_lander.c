#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Picks a site of a given type for a Lander. unk_09 is the range-map value of
 * the nearest foot unit, unk_0a that of a unit waiting for a T-Copter. A site
 * is passed over when unk_09 is above the best unk_0a so far, or when its unk_0a
 * ties the best and the best site had unk_0a at least its unk_09. Returns the
 * site index, or -1. */
int AiSite_PickForLander(int siteType) {
  AiSite* site;
  int i;
  int bestValue;
  int best;
  int bestWasFarther;

  bestWasFarther = 0;
  i = 0;
  site = data_0229e5bc;
  bestValue = 0x7fff;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestValue < site->unk_09) {
      continue;
    }
    if (bestValue == site->unk_0a && bestWasFarther == 1) {
      continue;
    }
    bestValue = site->unk_0a;
    best = i;
    if (site->unk_0a >= site->unk_09) {
      bestWasFarther = 1;
    } else {
      bestWasFarther = 0;
    }
  }
  return best;
}

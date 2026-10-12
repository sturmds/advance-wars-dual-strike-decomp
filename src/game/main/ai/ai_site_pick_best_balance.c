#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Picks the site with the highest unitBalance (enemy units near it minus own
 * team's), skipping skipped sites and those AiSite_NearUnguardedOozium rejects. The balance
 * must beat 0. Returns the site index, or -1. */
int AiSite_PickBestBalance(void) {
  AiSite* site;
  int i;
  int bestBalance;
  int best;

  bestBalance = 0;
  i = 0;
  site = data_0229e5bc;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (bestBalance < site->unitBalance) {
      bestBalance = site->unitBalance;
      best = i;
    }
  }
  return best;
}

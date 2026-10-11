#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int func_02096c60(int x, int y);

#define SITE_KIND_SKIPPED 0xfd
#define SITE_TYPE_ANY     4

/* Picks the site of a given type (SITE_TYPE_ANY: every type) with the lowest
 * unk_09, the range-map value of the nearest Infantry or Mech. Returns the
 * site index, or -1. */
int func_020969d0(int siteType)
{
    AiSite *site;
    int i;
    int bestValue;
    int best;

    site = data_0229e5bc;
    bestValue = 0x7fff;
    best = -1;
    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= SITE_KIND_SKIPPED) {
            continue;
        }
        if (func_02096c60(site->x, site->y) == 1) {
            continue;
        }
        if (siteType != SITE_TYPE_ANY && site->active != siteType) {
            continue;
        }
        if (bestValue > site->unk_09) {
            bestValue = site->unk_09;
            best = i;
        }
    }
    return best;
}

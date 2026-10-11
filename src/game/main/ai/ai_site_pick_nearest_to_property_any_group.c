#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int func_02096c60(int x, int y);

#define SITE_KIND_SKIPPED 0xfd
#define SITE_TYPE_ANY     4

/* AiSite_PickNearestToProperty without the group test. */
int func_02096a78(int siteType)
{
    AiSite *site;
    int i;
    int bestMoves;
    int best;

    site = data_0229e5bc;
    bestMoves = 0x7fff;
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
        if (bestMoves > site->propertyMoves) {
            bestMoves = site->propertyMoves;
            best = i;
        }
    }
    return best;
}

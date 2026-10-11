#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int func_02096c60(int x, int y);

#define SITE_KIND_SKIPPED 0xfd
#define SITE_TYPE_ANY     4

/* AiSite_PickMostEnemies without the group test. */
int func_02096b20(int siteType)
{
    AiSite *site;
    int i;
    int bestCount;
    int best;

    bestCount = 0;
    i = 0;
    site = data_0229e5bc;
    best = -1;
    for (; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= SITE_KIND_SKIPPED) {
            continue;
        }
        if (func_02096c60(site->x, site->y) == 1) {
            continue;
        }
        if (siteType != SITE_TYPE_ANY && site->active != siteType) {
            continue;
        }
        if (bestCount < site->enemyCount) {
            bestCount = site->enemyCount;
            best = i;
        }
    }
    return best;
}

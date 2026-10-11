#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int func_02096c60(int x, int y);

/* Returns the index of the first site of a given type (SITE_TYPE_ANY: every
 * type) that is not skipped and that func_02096c60 does not reject, or -1. */
int AiSite_PickFirst(int siteType)
{
    AiSite *site = data_0229e5bc;
    int i;

    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= SITE_KIND_SKIPPED) {
            continue;
        }
        if (func_02096c60(site->x, site->y) == 1) {
            continue;
        }
        if (siteType != SITE_TYPE_ANY && site->type != siteType) {
            continue;
        }
        return i;
    }
    return -1;
}

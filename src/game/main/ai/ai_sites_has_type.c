#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

#define SITE_TYPE_ANY 4

/* True when some AiSite record has the given type (SITE_TYPE_ANY: when there
 * is any record). Skipped records count. */
BOOL AiSites_HasType(int siteType)
{
    AiSite *site = data_0229e5bc;
    int i;

    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (siteType == SITE_TYPE_ANY || site->active == siteType) {
            return TRUE;
        }
    }
    return FALSE;
}

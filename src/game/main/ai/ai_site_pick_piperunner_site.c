#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Among the sites of type 1 that are not skipped and not rejected by
 * AiSite_NearUnguardedOozium, picks the one with the highest score (AiSite_ScorePiperunnerNeed).
 * Returns the site index, or -1 when no score is above 0. */
int AiSite_PickPiperunnerSite(void)
{
    AiSite *site;
    int i;
    int bestScore;
    int best;

    site = data_0229e5bc;
    bestScore = 0;
    best = -1;
    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= SITE_KIND_SKIPPED) {
            continue;
        }
        if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
            continue;
        }
        if (site->type != SITE_BASE) {
            continue;
        }
        if (bestScore < site->score) {
            bestScore = site->score;
            best = i;
        }
    }
    return best;
}

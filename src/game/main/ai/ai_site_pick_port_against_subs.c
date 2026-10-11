#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void Ai_CountEnemySubReach(void);
int AiSite_NearUnguardedOozium(int x, int y);
int AiBuild_RateSiteDanger(int x, int y, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define BUILD_RATING_MAX  60

/* Like AiSite_PickPortForCruiser, but after Ai_CountEnemySubReach: picks the
 * port whose tile enemy Subs reach the most (more than 0), among ports rated
 * below 60 for a Cruiser. The first of equal ports wins. Returns the site
 * index, or -1. */
int AiSite_PickPortAgainstSubs(void)
{
    AiSite *site;
    int i;
    int value;
    int bestValue;
    int best;

    site = data_0229e5bc;
    bestValue = 0;
    best = -1;
    Ai_CountEnemySubReach();
    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= SITE_KIND_SKIPPED) {
            continue;
        }
        if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
            continue;
        }
        if (site->type != SITE_PORT) {
            continue;
        }
        value = STATE->mapTiles->rangeScratch[TILE(site->x, site->y)];
        if (bestValue >= value) {
            continue;
        }
        if (AiBuild_RateSiteDanger(site->x, site->y, UNIT_CRUISER) < BUILD_RATING_MAX) {
            bestValue = value;
            best = i;
        }
    }
    return best;
}

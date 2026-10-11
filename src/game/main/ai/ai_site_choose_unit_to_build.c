#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_site.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 gAiBuildScores[];       /* per unit type: build score; 0: not a candidate */
extern u8 gUnitSupplyKind[];      /* per unit type: 1 ground, 2 air, 3 sea */

int UnitType_GetAttackKind(int unitType);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiBuild_RateSiteDanger(int x, int y, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Picks the unit type to build at a site: the type with the highest score
 * in gAiBuildScores, skipping ground indirect units, whose supply kind matches
 * the site type (base, airport or port).
 * A type the player cannot afford, or that fails, gets score 0 and the search
 * starts again. A type passes when AiBuild_RateSiteDanger rates it below 90; a ground
 * or sea type rated 90 or more gets one more pass that only asks for its cost
 * to be at most half the income or half the funds. Stores the type in
 * gUnitAction.target and returns siteIndex; -1 when nothing is left. */
int AiSite_ChooseUnitToBuild(int siteIndex)
{
    /* needed to match: declaration order sets the registers */
    Player *player;
    AiSite *site;
    int bestType;
    u32 bestScore;
    int retried;
    u32 cost;
    int type;

    site = &data_0229e5bc[siteIndex];
    retried = 0;
    /* a port, while some player or tile has air units or airports */
    if (site->type == SITE_PORT && (gAiTurnState->domainsAny & AI_DOMAIN_AIR)) {
        return -1;
    }
    for (;;) {
        bestType = 0;
        bestScore = 0;
        for (type = 1; type < UNIT_TYPE_COUNT; type++) {
            if (bestScore >= gAiBuildScores[type]) {
                continue;
            }
            /* needed to match: the (u8) cast */
            if (UnitType_GetAttackKind((u8)type) == ATTACK_KIND_INDIRECT
                && (Ov000_gUnitStats[type].moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
                continue;
            }
            bestScore = gAiBuildScores[type];
            bestType = type;
        }
        if (bestType == 0) {
            return -1;
        }
        if (site->type != gUnitSupplyKind[bestType]) {
            goto reject;
        }
        cost = (u16)(Unit_GetValuePerHp(STATE->currentPlayer, bestType) * 10);
        player = &STATE->players[STATE->currentPlayer];
        if (cost > player->funds) {
            goto reject;
        }
        if (!retried || (Ov000_gUnitStats[bestType].moveGroup & MOVE_GROUP_AIR)) {
            if (AiBuild_RateSiteDanger(site->x, site->y, bestType) < 90) {
                goto found;
            }
            if (Ov000_gUnitStats[bestType].moveGroup & MOVE_GROUP_AIR) {
                goto reject;
            }
            retried = 1;
            continue;
        }
        if (cost > player->unk_08 >> 1 && cost > (u32)player->funds >> 1) {
            goto reject;
        }
    found:
        gUnitAction.target = bestType;
        return siteIndex;
    reject:
        gAiBuildScores[bestType] = 0;
    }
}

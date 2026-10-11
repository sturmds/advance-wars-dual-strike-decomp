#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

extern u32 gAiBuildScores[];
extern u8 gUnitSupplyKind[];

int AiSite_PickFirst(int allowSaving);
int AiSite_RateBaseLandmass(void);
int UnitType_GetAttackKind(u8 unitType);
BOOL AiBuild_HasEnoughIndirectsForArtillery(int info);
BOOL AiBuild_HasEnoughGroundIndirects(int info);
int AiSite_PickForBuild(int supplyKind);
int AiBuild_RateSiteDanger(int x, int y, int unitType);
void AiSites_ClearRejectedMarks(void);
int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL AiBuild_ShouldSaveFunds(u32 cost);
int Ai_WantsBlackBomb(int unitType);
int AiSite_PickBlackBombAirport(void);
int AiSite_PickPiperunnerSite(void);
BOOL AiBuild_CanAffordPiperunnerInstead(int unitType);
BOOL AiBuild_WantsPiperunnerInstead(int unitType);
int Ai_CountUnitsOfType(int type);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (STATE->players[STATE->currentPlayer])

/* Called by func_02091904 (end-actions step 9). Picks the unit type with the highest weight in
 * gAiBuildScores and a factory for it (AiSite_PickForBuild by gUnitSupplyKind).
 * AiSite_RateBaseLandmass returns flags (bit 8) and a count (bits 0-7) that rule out
 * foot units or, for ground units, all but non-direct ones, Infantry, Mech and
 * Anti-Air (at half weight). An airport or port where AiBuild_RateSiteDanger gives 90 or
 * more is marked (kind 0xff) and the next one tried. When funds are short and
 * allowSaving is 1, AiBuild_ShouldSaveFunds may set AiBuildState.savingTarget to the funds
 * still missing after next income; while savingTarget is set, a build must leave
 * that much (savingSkips times it may skip the check for a type the player has
 * none of). Black Bomb and Piperunner builds go through AiSite_PickBlackBombAirport and
 * AiSite_PickPiperunnerSite. A type that fails gets weight 0 and the pick starts again.
 * Returns the factory's site index with gUnitAction.target set, or -1. */
int AiTurn_PickUnitToBuild(int allowSaving)
{
    /* needed to match: this declaration order and the hoisted countLess1 */
    int index;
    u8 *supplyKind;
    int best;
    int infoFlag;
    u32 bestScore;
    int pipeCost;
    int info;
    int type;
    UnitStats *stats;
    int countLess1;
    AiSite *site;
    int infoCount;
    u16 cost;

    if (AiSite_PickFirst(4) == -1) {
        return -1;
    }
    info = AiSite_RateBaseLandmass();
    infoFlag = info & 0x100;
    infoCount = info & 0xff;
    for (;;) {
        best = 0;
        bestScore = 0;
        countLess1 = infoCount - 1;
        for (type = 1, stats = &Ov000_gUnitStats[1]; type < UNIT_TYPE_COUNT; stats++, type++) {
            if (bestScore >= gAiBuildScores[type]) {
                continue;
            }
            if (infoFlag != 0) {
                if (type <= UNIT_MECH && countLess1 >= 5) {
                    continue;
                }
            } else if (infoCount != 0 && (stats->moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
                if (UnitType_GetAttackKind(type) != ATTACK_KIND_DIRECT || type == UNIT_INFANTRY || type == UNIT_MECH || type == UNIT_ANTI_AIR) {
                    if (type == UNIT_ARTILLERY) {
                        continue;
                    }
                    if (type <= UNIT_MECH && countLess1 >= 5) {
                        continue;
                    }
                    if (bestScore <= gAiBuildScores[type] >> 1) {
                        bestScore = gAiBuildScores[type] >> 1;
                        best = type;
                    }
                }
                continue;
            }
            if (type == UNIT_ARTILLERY) {
                if (AiBuild_HasEnoughIndirectsForArtillery(info) == 1) {
                    continue;
                }
            } else if (UnitType_GetAttackKind(type) == ATTACK_KIND_INDIRECT
                       && (stats->moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))
                       && AiBuild_HasEnoughGroundIndirects(info) == 1) {
                continue;
            }
            best = type;
            bestScore = gAiBuildScores[type];
        }
        if (best == 0) {
            return -1;
        }
        supplyKind = &gUnitSupplyKind[best];
        for (;;) {
            index = AiSite_PickForBuild(*supplyKind);
            if (index == -1) {
                goto reject;
            }
            /* needed to match: airport or port written as + 254 */
            if ((u8)(*supplyKind + 254) > 1) {
                break;
            }
            site = &data_0229e5bc[index];
            if (AiBuild_RateSiteDanger(site->x, site->y, best) >= 90) {
                site->kind = 0xff;
                continue;
            }
            AiSites_ClearRejectedMarks();
            break;
        }
        cost = Unit_GetValuePerHp(STATE->currentPlayer, best) * 10;
        if (cost > (u32)PLAYER.funds) {
            if (allowSaving == 1 && AiBuild_ShouldSaveFunds(cost) == 1) {
                data_021619c0->savingTarget = cost - (PLAYER.unk_08 - data_021619c0->unk_0c);
            }
            goto reject;
        }
        if (Ai_WantsBlackBomb(best) == 1) {
            gUnitAction.target = UNIT_BLACK_BOMB;
            return AiSite_PickBlackBombAirport();
        }
        if (data_021619c0->savingTarget != 0) {
            if (AiBuild_CanAffordPiperunnerInstead(best) == 1) {
                gUnitAction.target = UNIT_PIPERUNNER;
                return AiSite_PickPiperunnerSite();
            }
        } else if (AiBuild_WantsPiperunnerInstead(best) == 1) {
            pipeCost = Unit_GetValuePerHp(STATE->currentPlayer, UNIT_PIPERUNNER) * 10;
            if (pipeCost > (u32)PLAYER.funds) {
                if (allowSaving == 1 && AiBuild_ShouldSaveFunds(pipeCost) == 1) {
                    data_021619c0->savingTarget = pipeCost - (PLAYER.unk_08 - data_021619c0->unk_0c);
                    continue;
                }
            } else {
                gUnitAction.target = UNIT_PIPERUNNER;
                return AiSite_PickPiperunnerSite();
            }
        }
        if (data_021619c0->savingTarget != 0 && data_021619c0->savingSkips != 0) {
            data_021619c0->savingSkips--;
            if (Ai_CountUnitsOfType(best) != 0 && cost > PLAYER.funds - data_021619c0->savingTarget) {
                goto reject;
            }
        } else if (cost > PLAYER.funds - data_021619c0->savingTarget) {
            goto reject;
        }
        gUnitAction.target = best;
        return index;
    reject:
        gAiBuildScores[best] = 0;
        AiSites_ClearRejectedMarks();
    }
}

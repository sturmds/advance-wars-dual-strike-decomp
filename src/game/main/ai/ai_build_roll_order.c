#include "types.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Ai_HasOwnBaseNearHq(void);
BOOL Ai_HasEnemyWithin(s16 x, s16 y, int radius);
int Rng_Next(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_PLAYER (STATE->players[STATE->currentPlayer])

#define ORDER_COUNT 7
#define HQ_SAFE_RADIUS 6

/* Picks the MapUnit.aiOrder (0-6) of a unit type about to be built, at
 * random with the weights in gAiProfile->units[unitType - 1].orderWeights. Bits of
 * `excluded` give orders weight 0. Order 6 is also left out when the current
 * player has a base near the HQ (Ai_HasOwnBaseNearHq) and no enemy within
 * HQ_SAFE_RADIUS of the HQ. Returns 0 for sea units and for types whose first
 * weight is negative. The caller passes the result through gUnitAction.unk_07
 * to AiBuild_BuyAtSite. */
u8 AiBuild_RollOrder(int unitType, u8 excluded)
{
    u8 weights[ORDER_COUNT];
    u32 total;
    u8 index = unitType - 1;
    int i;
    u8 roll;

    if (gAiProfile->units[index].orderWeights[0] < 0) {
        return 0;
    }
    if (Ov000_gUnitStats[index + 1].moveGroup & MOVE_GROUP_SEA) {
        return 0;
    }
    if (Ai_HasOwnBaseNearHq() == TRUE) {
        if (!Ai_HasEnemyWithin(CURRENT_PLAYER.hqX & 0x7f, CURRENT_PLAYER.hqY & 0x7f, HQ_SAFE_RADIUS)) {
            excluded |= 1 << 6;
        }
    }
    total = 0;
    for (i = 0; i < ORDER_COUNT; i++) {
        if (excluded & (1 << i)) {
            weights[i] = 0;
        } else {
            weights[i] = gAiProfile->units[index].orderWeights[i];
            total += weights[i];
        }
    }
    roll = (u32)Rng_Next() % total;
    for (i = 0; i < ORDER_COUNT; i++) {
        if (weights[i] > roll) {
            return i;
        }
        roll -= weights[i];
    }
    return 0;
}

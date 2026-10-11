#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit *unit);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int AiUnit_ClaimLandmass(void);
int AiUnit_CanReachCaptureSite(MapUnit *unit);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* Called by AiFoot_HeadForCaptureSite when no capture site is in range.
 * The acting foot unit boards an own transport with room that it can
 * reach: an APC, T-Copter, Black Boat or Lander. Later finds replace
 * earlier ones, except that a T-Copter is never replaced, a Lander is not
 * replaced by an APC, and a Black Boat is replaced only by another Black
 * Boat or a T-Copter. An APC is taken only if AiUnit_CanReachCaptureSite accepts it. A
 * boat gets the tile as its AI target. */
void AiFoot_BoardTransport(void)
{
    int boatX = -1;
    int boatY = -1;
    int boatType = 0;
    int x;
    int y;
    MapUnit *transport;
    int type;

    if (STATE->actingUnit->claimedLandmass || AiUnit_ClaimLandmass() == 1) {
        STATE->actingUnit->transportWanted = 0;
        return;
    }
    RangeMap_FillUnitMoves(STATE->actingUnit);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            if (MAP->unitAt[TILE_INDEX(x, y)] == 0) {
                continue;
            }
            if (!Unit_IsCurrentPlayers(MAP->unk_508[TILE_INDEX(x, y)])) {
                continue;
            }
            transport = &STATE->units[MAP->unitAt[TILE_INDEX(x, y)]];
            type = transport->type;
            switch (type) {
            case UNIT_APC:
            case UNIT_T_COPTER:
                if (transport->cargo[0] != 0) {
                    continue;
                }
                break;
            case UNIT_BLACK_BOAT:
            case UNIT_LANDER:
                if (transport->cargo[1] != 0) {
                    continue;
                }
                break;
            default:
                continue;
            }
            switch (boatType) {
            case UNIT_NONE:
            case UNIT_APC:
                break;
            case UNIT_BLACK_BOAT:
                if (type == UNIT_LANDER) {
                    continue;
                }
            case UNIT_LANDER:
                if (type == UNIT_APC) {
                    continue;
                }
                break;
            case UNIT_T_COPTER:
            default:
                continue;
            }
            boatX = x;
            boatY = y;
            boatType = type;
        }
    }
    if (boatX == -1) {
        return;
    }
    if (boatType == UNIT_APC) {
        if (AiUnit_CanReachCaptureSite(&STATE->units[MAP->unitAt[TILE_INDEX(boatX, boatY)]]) == 0) {
            return;
        }
    }
    if (boatType == UNIT_LANDER || boatType == UNIT_BLACK_BOAT) {
        transport = &STATE->units[MAP->unitAt[TILE_INDEX(boatX, boatY)]];
        transport->aiTargetX = boatX;
        transport->aiTargetY = boatY;
    }
    STATE->actingUnit->transportWanted = 0;
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(boatX, boatY, AI_ACTION_BOARD, 0, 0);
}

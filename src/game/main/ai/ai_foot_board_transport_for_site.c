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
/* the site entry for the tile the acting unit is heading to */
#define TARGET_SITE (STATE->unk_6088[MAP->unk_1928[TILE_INDEX(STATE->actingUnit->aiTargetX, STATE->actingUnit->aiTargetY)]])

/* Called by AiFoot_HeadForCaptureSite when the capture site is more than 2
 * tiles away (siteDistance). Like AiFoot_BoardTransport, but the unit also
 * gives up its claim on its old target site (Unk_02080440_6088.claimCount) when
 * it takes a boat. A Lander or Black Boat is skipped when the unit has
 * unk_0c_4 set or the site is 4 tiles or less away, and a Black Boat or
 * Lander holding the pick is not replaced by an APC. */
void AiFoot_BoardTransportForSite(int siteDistance)
{
    int boatX = -1;
    int boatY = -1;
    int boatType = 0;
    int x;
    int y;
    MapUnit *transport;
    int type;

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
                if (STATE->actingUnit->unk_0c_4 == 1) {
                    continue;
                }
                if (type == UNIT_APC) {
                    continue;
                }
                break;
            case UNIT_T_COPTER:
            default:
                continue;
            }
            if (STATE->actingUnit->unk_0c_4 == 1 || siteDistance <= 4) {
                if (type == UNIT_LANDER || type == UNIT_BLACK_BOAT) {
                    continue;
                }
            }
            boatX = x;
            boatY = y;
            boatType = transport->type; /* needed to match: not type */
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
        if (STATE->actingUnit->aiTargetX != MAP_UNIT_NO_TARGET) {
            if (TARGET_SITE.claimCount != 0) {
                TARGET_SITE.claimCount--;
            }
        }
        transport = &STATE->units[MAP->unitAt[TILE_INDEX(boatX, boatY)]];
        transport->aiTargetX = boatX;
        transport->aiTargetY = boatY;
    }
    STATE->actingUnit->transportWanted = 0;
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(boatX, boatY, AI_ACTION_BOARD, 0, 0);
}

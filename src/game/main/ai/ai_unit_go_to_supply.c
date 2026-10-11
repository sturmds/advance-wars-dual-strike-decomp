#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind[]; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL FindAttackTile(int x, int y, s16 *pos);
BOOL Unit_CanRepairAt(MapUnit *unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit *unit, int x, int y);
BOOL AiUnit_HasFuelForTile(s16 x, s16 y, int range);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
void Ai_ClearActingUnitTarget(void);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

#define CARRY_TABLE(unitType) (Ov000_gUnitStats[unitType].carryTypes)

/* Run when the acting unit's serviceWanted is 1, the value that AiClass3_SupplyUnit
 * and AiBlackBoat_RepairUnit look for. Moves the unit to where it is
 * supplied or repaired. A waited APC in reach: the unit moves next to it and
 * waits. A Cruiser or Carrier in reach (rangeMap value up to gAiTurnState->unk_05)
 * that takes this unit type aboard and has its second cargo slot free: action 7
 * on its tile. Otherwise it scores empty tiles where it is repaired
 * (Unit_CanRepairAt or Unit_IsBlackHoleOnFlaggedTile; a tile for Unit_IsBlackHoleOnFlaggedTile scores 0) by
 * rangeMap value, plus unk_05 on a base, airport or port, and heads for the
 * lowest, waiting there when it can reach it. needed to match: the Cruiser
 * and Carrier cases are written out twice. */
void AiUnit_GoToSupply(void)
{
    int bestX = -1;
    u32 bestY = -1; /* needed to match: found by tools/permute.py */
    int bestScore = NO_TILE;
    int slot;
    MapUnit *unit;
    int score;
    int x;
    int y;
    s16 pos[2];

    RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, STATE->actingUnit->type, UNLIMITED_MOVE, 1);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            slot = MAP->unitAt[MAP->rowStart[y] + x];
            if (slot != 0) {
                unit = &STATE->units[slot];
                switch (unit->type) {
                case UNIT_APC:
                    if ((unit->flags & MAP_UNIT_WAITED) && FindAttackTile(x, y, pos)) {
                        AiUnit_PerformAction(pos[0], pos[1], AI_ACTION_WAIT, 0, 0);
                    }
                    break;
                case UNIT_CRUISER:
                    if (STATE->rangeMap[y][x] <= gAiTurnState->unk_05
                        && (CARRY_TABLE(unit->type) + STATE->actingUnit->type)[1] != 0 && unit->cargo[1] == 0) {
                        AiUnit_PerformAction(x, y, AI_ACTION_BOARD, 0, 0);
                    }
                    break;
                case UNIT_CARRIER:
                    if (STATE->rangeMap[y][x] <= gAiTurnState->unk_05
                        && (CARRY_TABLE(unit->type) + STATE->actingUnit->type)[1] != 0 && unit->cargo[1] == 0) {
                        AiUnit_PerformAction(x, y, AI_ACTION_BOARD, 0, 0);
                    }
                    break;
                }
                continue;
            }
            if (!Unit_CanRepairAt(STATE->actingUnit, x, y) && !Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y)) {
                continue;
            }
            if (!AiUnit_HasFuelForTile(x, y, gAiTurnState->unk_05)) {
                continue;
            }
            score = STATE->rangeMap[y][x];
            if (gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)])] != 0) {
                score += gAiTurnState->unk_05;
            }
            if (Ai_IsTileNeededByAlly(x, y) == 1) {
                continue;
            }
            if (Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == 1) {
                score = 0;
            }
            if (score <= bestScore) {
                bestScore = score;
                bestX = x;
                bestY = y;
            }
        }
    }
    if (bestScore == NO_TILE) {
        return;
    }
    Ai_ClearActingUnitTarget();
    if (STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
        AiUnit_PerformAction(bestX, bestY, AI_ACTION_WAIT, 0, 0);
    } else {
        AiUnit_MoveTowardTile(bestX, bestY);
    }
}

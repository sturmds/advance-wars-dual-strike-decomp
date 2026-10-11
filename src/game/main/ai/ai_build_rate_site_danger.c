#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int UnitType_BestBaseDamage(int unitType, int targetType);
int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
int Unit_GetCoMaxRange(MapUnit *unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define OWNER(unit) MAP_UNIT_OWNER(STATE, unit)

/* Rates the danger to a unit of unitType built at (x, y), for the AI's build
 * choice (AiSite_ChooseUnitToBuild and AiTurn_PickUnitToBuild compare it with
 * 90). For an air unit it first adds up the hp of the current team's Fighters
 * that can reach the site (cover). Then each enemy unit 1 to 9 tiles away adds
 * best base damage * hp / 100 when it can hit the site:
 * - an enemy Fighter or Stealth against an air unit, while cover is below
 *   100, when it can move next to the site; against a Fighter or Stealth only
 *   the largest such threat counts;
 * - a Cruiser against air units and Black Boats, Anti-Air against air units
 *   (half for a Bomber or Stealth), a Sub against Battleships and Carriers,
 *   when it can move next to the site;
 * - any unit with a max range above 1 that has the site in range.
 *
 * Not matching yet (97.5%): mwcc gives `state` stack slot 0x24, the original
 * 0x38 (after the compiler temporaries), so every slot from 0x24 to 0x34
 * moves by 4. Declaration order, block scope and the hybrids of `state` and
 * STATE did not move it. Without `state`, the slot order is right but Fighter
 * units are read through the outer STATE copy. */
int AiBuild_RateSiteDanger(int x, int y, int unitType)
{
    /* needed to match: declaration order sets the stack slots */
    int maxY;
    int damage;
    int maxX;
    int total;
    int minY;
    int ty;
    int col;
    int minX;
    u8 slot;
    int row;
    int airMax;
    u16 savedUnk1e;
    MapUnit *unit;
    u16 cover;
    Unk_02080440_48 *state;
    int tx;
    int dist;
    int range;

    total = 0;
    airMax = 0;
    cover = 0;
    state = STATE;
    savedUnk1e = state->unk_1e;
    if (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) {
        RangeMap_FillMoves(x, y, UNIT_FIGHTER, Player_GetUnitMoveRange(state->currentPlayer, UNIT_FIGHTER), 1);
        RangeMap_MarkAdjacentTiles(111);
        state = STATE;
        for (row = 0; row < state->unk_1c4->mapHeight; row++) {
            for (col = 0; col < state->unk_1c4->mapWidth; col++) {
                if (state->rangeMap[row][col] < 0) {
                    continue;
                }
                slot = STATE->mapTiles->unk_508[TILE_INDEX(col, row)];
                if (slot == 0) {
                    continue;
                }
                if (STATE->players[STATE->currentPlayer].team != STATE->players[(STATE->mapTiles->unitAt[TILE_INDEX(col, row)] >> 6) + 1].team) {
                    continue;
                }
                unit = &STATE->units[slot];
                if (unit->type != UNIT_FIGHTER) {
                    continue;
                }
                cover += unit->hp;
            }
        }
    }
    minX = x - 9;
    if (minX < 0) {
        minX = 0;
    }
    minY = y - 9;
    if (minY < 0) {
        minY = 0;
    }
    maxX = state->unk_1c4->mapWidth - 1;
    if (x + 9 < maxX) {
        maxX = x + 9;
    }
    maxY = state->unk_1c4->mapHeight - 1;
    if (y + 9 < maxY) {
        maxY = y + 9;
    }
    for (ty = minY; ty <= maxY; ty++) {
        for (tx = minX; tx <= maxX; tx++) {
            dist = 0;
            if (x - tx > 0) {
                dist += x - tx;
            } else {
                dist += tx - x;
            }
            if (y - ty > 0) {
                dist += y - ty;
            } else {
                dist += ty - y;
            }
            if (dist < 1 || dist > 9) {
                continue;
            }
            slot = STATE->mapTiles->unk_508[TILE_INDEX(tx, ty)];
            if (slot == 0) {
                continue;
            }
            if (STATE->players[STATE->currentPlayer].team == STATE->players[(STATE->mapTiles->unitAt[TILE_INDEX(tx, ty)] >> 6) + 1].team) {
                continue;
            }
            unit = &STATE->units[slot];
            if (cover < 100 && (unit->type == UNIT_FIGHTER || unit->type == UNIT_STEALTH)
                && (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR)) {
                STATE->unk_1e = OWNER(unit);
                RangeMap_FillMoves(unit->x, unit->y, unit->type, Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
                RangeMap_MarkAdjacentTiles(111);
                if (STATE->rangeMap[y][x] < 0) {
                    continue;
                }
                damage = HwDiv32(UnitType_BestBaseDamage(unit->type, unitType) * unit->hp, 100);
                if (unitType != UNIT_FIGHTER && unitType != UNIT_STEALTH) {
                    total += damage;
                } else if (damage > airMax) {
                    airMax = damage;
                }
                continue;
            }
            if (((Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) || unitType == UNIT_BLACK_BOAT) && unit->type == UNIT_CRUISER
                || (Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) && unit->type == UNIT_ANTI_AIR
                || (unitType == UNIT_BATTLESHIP || unitType == UNIT_CARRIER) && unit->type == UNIT_SUB) {
                STATE->unk_1e = OWNER(unit);
                RangeMap_FillMoves(unit->x, unit->y, unit->type, Player_GetUnitMoveRange(OWNER(unit), unit->type), 1);
                RangeMap_MarkAdjacentTiles(111);
                if (STATE->rangeMap[y][x] < 0) {
                    continue;
                }
                damage = HwDiv32(UnitType_BestBaseDamage(unit->type, unitType) * unit->hp, 100);
                if ((unitType == UNIT_BOMBER || unitType == UNIT_STEALTH) && unit->type == UNIT_ANTI_AIR) {
                    damage >>= 1;
                }
                total += damage;
                continue;
            }
            range = Unit_GetCoMaxRange(unit);
            if (range <= 1 || range < dist || Ov000_gUnitStats[unit->type].minRange > dist) {
                continue;
            }
            total += HwDiv32(UnitType_BestBaseDamage(unit->type, unitType) * unit->hp, 100);
        }
    }
    STATE->unk_1e = savedUnk1e;
    return total + airMax;
}

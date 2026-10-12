#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/range_map.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Terrain_IsProperty(u8 tile);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL AiSite_UnitCanReach(int siteIndex, MapUnit *unit);
BOOL Unit_IsHarmless(MapUnit *unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define UNIT_OWNER(slot) (((slot) >> 6) + 1)

#define FAR_AWAY 110

/* Rates the area around each AiSite of one group:
 * - enemyDistance: distance to the nearest enemy unit that AiSite_UnitCanReach lets
 *   count for the site.
 * - unitBalance, enemyCount: units within gAiProfile->unk_08[8] tiles that a
 *   Tank could reach, skipping unarmed units with no cargo (Unit_IsHarmless).
 * - propertyMoves (when still 0xff): Infantry move cost to the nearest
 *   property the current team does not own. */
void AiSiteGroup_RateSurroundings(int group)
{
    AiSite *site;
    int maxX;
    int maxY;
    int dist;
    u8 range;
    int y;
    MapUnit *unit;
    int i;
    int minY;
    u8 slot;
    int x;
    int minX;
    int propertyX;                     /* needed for the register order: the last loop gets its own x */

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            slot = STATE->mapTiles->unitAt[TILE(x, y)];
            if (slot == 0) {
                continue;
            }
            unit = &STATE->units[slot];
            if (STATE->players[STATE->currentPlayer].team == STATE->players[UNIT_OWNER(slot)].team) {
                continue;
            }
            site = data_0229e5bc;
            for (i = 0; i < data_021619c0->recordCount; i++, site++) {
                if (site->group != group) {
                    continue;
                }
                if (!AiSite_UnitCanReach(i, unit)) {
                    continue;
                }
                dist = Map_GetDistance(site->x, site->y, x, y);
                if (site->enemyDistance > dist) {
                    site->enemyDistance = dist;
                }
            }
        }
    }

    site = data_0229e5bc;
    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->group != group) {
            continue;
        }
        RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
        RangeMap_FillMoves(site->x, site->y, UNIT_TANK, gAiProfile->unk_08[8], 0);
        RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);

        range = gAiProfile->unk_08[8];
        /* Needed to match: these clamp forms and the declaration order set
         * the spill slots and the order the hoisted zeros are stored in. */
        if (site->x - range < 0) {
            minX = 0;
        } else {
            minX = site->x - range;
        }
        if (site->y - range < 0) {
            minY = 0;
        } else {
            minY = site->y - range;
        }
        if (site->x + range >= STATE->unk_1c4->mapWidth - 1) {
            maxX = STATE->unk_1c4->mapWidth - 1;
        } else {
            maxX = site->x + range;
        }
        if (site->y + range >= STATE->unk_1c4->mapHeight - 1) {
            maxY = STATE->unk_1c4->mapHeight - 1;
        } else {
            maxY = site->y + range;
        }
        for (y = minY; y <= maxY; y++) {
            for (x = minX; x <= maxX; x++) {
                dist = 0;
                if (site->x - x > 0) {
                    dist += site->x - x;
                } else {
                    dist += x - site->x;
                }
                if (site->y - y > 0) {
                    dist += site->y - y;
                } else {
                    dist += y - site->y;
                }
                if (dist < 0 || dist > gAiProfile->unk_08[8]) {
                    continue;
                }
                slot = STATE->mapTiles->unitAt[TILE(x, y)];
                if (slot == 0) {
                    continue;
                }
                unit = &STATE->units[slot];
                if (!AiSite_UnitCanReach(i, unit)) {
                    continue;
                }
                if (Unit_IsHarmless(unit) == TRUE) {
                    continue;
                }
                if ((Ov000_gUnitStats[unit->type].moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))
                    && STATE->mapTiles->rangeScratch[TILE(x, y)] < 0) {
                    continue;
                }
                if (STATE->players[STATE->currentPlayer].team == STATE->players[UNIT_OWNER(slot)].team) {
                    site->unitBalance--;
                    continue;
                }
                if (site->type == SITE_PORT || unit->type <= UNIT_MECH) {
                    site->unitBalance += 1;
                } else {
                    site->unitBalance += 2;
                }
                site->enemyCount++;
            }
        }

        if (site->propertyMoves != 0xff) {
            continue;
        }
        RangeMap_FillMoves(site->x, site->y, UNIT_INFANTRY, FAR_AWAY, 0);
        for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
            for (propertyX = 0; propertyX < STATE->unk_1c4->mapWidth; propertyX++) {
                if (STATE->rangeMap[y][propertyX] < 0) {
                    continue;
                }
                if (Terrain_IsProperty(STATE->mapTiles->tiles[TILE(propertyX, y)]) != TRUE) {
                    continue;
                }
                /* the original reads currentUnitBase with ldrsh */
                if (Unit_IsTileOwnedByTeam((s16)STATE->currentUnitBase, STATE->mapTiles->tiles[TILE(propertyX, y)])) {
                    continue;
                }
                if (site->propertyMoves > STATE->rangeMap[y][propertyX]) {
                    site->propertyMoves = STATE->rangeMap[y][propertyX];
                }
            }
        }
    }
}

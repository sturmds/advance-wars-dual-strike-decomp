#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/range_map.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(s8 mark);
int UnitType_GetMaxRange(int unitType);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define TERRAIN_PIPE_FIRST 15          /* guess: pipe and pipe seam */
#define SITE_SKIPPED       0xfd

void func_0209462c(void)
{
    int dist;
    int minY;
    u32 siteX;
    int range;
    int maxY;
    u16 minX;
    int slot;
    int y;
    AiSite *site;
    u8 siteY;
    int x;
    int terrain;
    int i;
    MapUnit *unit;
    int maxX;
    s8 found;
    s32 ownCount;
    int enemyCount;

    site = data_0229e5bc;
    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind == SITE_SKIPPED || site->active != 1) {
            continue;
        }
        siteX = site->x;
        siteY = site->y;

        minX = siteX - 1;
        found = 0;
        if (minX < 0) {
            minX = 0;
        }
        minY = siteY - 1;
        if (minY < 0) {
            minY = 0;
        }
        maxX = STATE->unk_1c4->mapWidth - 1;
        if (siteX + 1 < maxX) {
            maxX = siteX + 1;
        }
        maxY = STATE->unk_1c4->mapHeight - 1;
        if (siteY + 1 < maxY) {
            maxY = siteY + 1;
        }
        for (y = minY; y <= maxY; y++) {
            for (x = minX; x <= maxX; x++) {
                dist = 0;
                if (siteX - x > 0) {
                    dist += siteX - x;
                } else {
                    dist += x - siteX;
                }
                if (siteY - y > 0) {
                    dist += siteY - y;
                } else {
                    dist += y - siteY;
                }
                if (dist < 1 || dist > 1) {
                    continue;
                }
                terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]);
                if (terrain - TERRAIN_PIPE_FIRST <= 1U) {
                    found = 1;
                }
            }
        }
        if (!found) {
            continue;
        }

        RangeMap_FillMoves(siteX, siteY, UNIT_PIPERUNNER, 110, 1);
        for (range = 0; range < UnitType_GetMaxRange(UNIT_PIPERUNNER); range++) {
            RangeMap_MarkAdjacentTiles(111 + range);
        }

        ownCount = 0;
        enemyCount = 0;
        for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
            for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
                if (STATE->rangeMap[y][x] < 0) {
                    continue;
                }
                slot = STATE->mapTiles->unitAt[TILE(x, y)];
                if (slot == 0) {
                    continue;
                }
                if (Unit_IsCurrentPlayers(slot)) {
                    if (STATE->rangeMap[y][x] > 110) {
                        continue;
                    }
                    if (STATE->units[slot].type == UNIT_PIPERUNNER) {
                        ownCount++;
                    }
                } else {
                    if (STATE->players[STATE->currentPlayer].team == STATE->players[(slot >> 6) + 1].team) {
                        continue;
                    }
                    unit = &STATE->units[slot];
                    if (UnitType_GetMaxRange(unit->type) != 0) {
                        if (unit->type == UNIT_PIPERUNNER) {
                            enemyCount += 4;
                        } else {
                            enemyCount += 1;
                        }
                    }
                }
            }
        }
        site->score = ((enemyCount + 4) >> 3) - ownCount;
    }
}

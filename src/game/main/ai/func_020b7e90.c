#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int func_020b80d0(MapUnit *unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define ABS_DIFF(a, b) ((a) - (b) > 0 ? (a) - (b) : (b) - (a))

/* Radius of a Black Bomb explosion in tiles. */
#define BLOMB_RADIUS 3

/* Adds up func_020b80d0 over the units within 3 tiles (by taxicab
 * distance, not on the centre tile) of (x, y). Skipped: Black Bombs,
 * Oozium, unarmed units, units at 10 hp or less and tiles out of sight. */
int func_020b7e90(int x, int y)
{
    int total = 0;
    int minX = x - BLOMB_RADIUS;
    int minY;
    int maxX;
    int maxY;
    int tx;
    int ty;
    int dist;
    int rowDist;
    MapUnit *unit;

    if (minX < 0) {
        minX = 0;
    }
    minY = y - BLOMB_RADIUS;
    if (minY < 0) {
        minY = 0;
    }
    maxX = x + BLOMB_RADIUS;
    if (maxX > STATE->unk_1c4->mapWidth - 1) {
        maxX = STATE->unk_1c4->mapWidth - 1;
    }
    maxY = y + BLOMB_RADIUS;
    if (maxY > STATE->unk_1c4->mapHeight - 1) {
        maxY = STATE->unk_1c4->mapHeight - 1;
    }
    for (ty = minY; ty <= maxY; ty++) {
        rowDist = 0;
        for (tx = minX; tx <= maxX; tx++) {
            dist = rowDist + ABS_DIFF(x, tx);
            dist += ABS_DIFF(y, ty);
            if (dist < 1 || dist > BLOMB_RADIUS) {
                continue;
            }
            unit = &STATE->units[MAP->unitAt[TILE_INDEX(tx, ty)]];
            if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM || unit->type == UNIT_BLACK_BOMB) {
                continue;
            }
            if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
                continue;
            }
            if (unit->hp <= 10) {
                continue;
            }
            if (!Map_IsTileVisible(unit->x, unit->y)) {
                continue;
            }
            total += func_020b80d0(unit);
        }
    }
    return total;
}

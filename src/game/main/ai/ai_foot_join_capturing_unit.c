#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit *unit);
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile);      /* TRUE: HQ, airport, port, base or com tower */
BOOL Unit_IsCurrentPlayers(s16 unitSlot); /* TRUE: the slot belongs to the current player */
s32 HwDiv32(s32 numerator, s32 denominator);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Joins the acting unit with one of our own units of the same type that is
 * capturing and not at full hp, on any tile the acting unit can reach. HP here
 * is the shown value, 1-10. The join is queued when the sum is at most 15 on
 * an HQ, airport, port, base or com tower, at most 12 when the acting unit
 * shows under 7, or at most 10. */
void AiFoot_JoinCapturingUnit(void)
{
    MapUnit *unit;
    int x;
    int y;
    int slot;
    int targetHp;
    int ownHp;

    RangeMap_FillUnitMoves(STATE->actingUnit);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            slot = STATE->mapTiles->unitAt[TILE(x, y)];
            if (slot == 0) {
                continue;
            }
            if (!Unit_IsCurrentPlayers(slot)) {
                continue;
            }
            unit = &STATE->units[slot];
            if (unit->captureProgress == 0) {
                continue;
            }
            if (unit->type != STATE->actingUnit->type) {
                continue;
            }
            if (unit->hp == 0) {
                targetHp = 0;
            } else {
                targetHp = HwDiv32(unit->hp - 1, 10) + 1;
            }
            if (targetHp == 10) {
                continue;
            }
            if (STATE->actingUnit->hp == 0) {
                ownHp = 0;
            } else {
                ownHp = HwDiv32(STATE->actingUnit->hp - 1, 10) + 1;
            }
            if (Terrain_IsHqBaseAirportPortOrTower(STATE->mapTiles->tiles[TILE(unit->x, unit->y)]) && targetHp + ownHp <= 15) {
                AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
            }
            if (ownHp < 7 && targetHp + ownHp <= 12) {
                AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
            }
            if (targetHp + ownHp <= 10) {
                AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
            }
        }
    }
}

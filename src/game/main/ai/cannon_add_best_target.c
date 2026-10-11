#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_02167eb4[];

#define STATE (data_027c027c->unk_48[data_027c0284])
/* needed to match: the original reads the slot again for each use. */
#define UNIT_SLOT_AT(x, y) (STATE->mapTiles->unitAt[(x) + STATE->mapTiles->rowStart[(y)]])

void RangeMap_SetBuffer(void *tileMap);
void RangeMap_Fill(int value);
int MapObject_GetDrawOffsetX(int kind, int variant);
int MapObject_GetDrawOffsetY(int kind, int variant);
void RangeMap_FillCone(u16 x, u16 y, u16 variant, u16 size, int value);
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
int DamageFromCannons(int playerIndex, int coSlot);
void AiTargetList_Clear(void);
void AiTargetList_Add(u16 a, u16 b, u16 unitSlot, u16 c);

/* Picks the target of a cannon object (kind, variant) at (x, y): the visible
 * enemy unit in its range (marked by RangeMap_FillCone) worth the most, hp times
 * cost / 100, with ties going to the later tile. Adds it to the AI target
 * list with damage less the target owner's DamageFromCannons reduction, or
 * empties the list when no unit is in range. */
void Cannon_AddBestTarget(int x, int y, int damage, int kind, int variant)
{
    MapUnit *unit;
    u32 bestValue;
    u32 value;
    u16 bestX;
    u16 bestY;
    u8 bestSlot;
    u8 slot;
    int tx;
    int ty;
    int dx;
    int reduction;

    bestValue = 0;
    bestX = 0;
    bestY = 0;
    bestSlot = 0;
    RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
    RangeMap_Fill(-1);
    dx = MapObject_GetDrawOffsetX(kind, variant);
    RangeMap_FillCone(x + dx, y + MapObject_GetDrawOffsetY(kind, variant), variant, data_02167eb4[kind], 0);

    for (ty = 0; ty < STATE->unk_1c4->mapHeight; ty++) {
        for (tx = 0; tx < STATE->unk_1c4->mapWidth; tx++) {
            if (STATE->rangeMap[ty][tx] < 0) {
                continue;
            }
            slot = UNIT_SLOT_AT(tx, ty);
            if (slot == 0) {
                continue;
            }
            if (STATE->players[STATE->currentPlayer].team == STATE->players[(slot >> 6) + 1].team) {
                continue;
            }
            if (!Player_CanSeeUnitAt(STATE->currentPlayer, tx, ty)) {
                continue;
            }
            unit = &STATE->units[UNIT_SLOT_AT(tx, ty)];
            if (unit->flags & MAP_UNIT_DIVED) {
                continue;
            }
            if (!Unit_IsVisibleToCurrentPlayer(UNIT_SLOT_AT(tx, ty))) {
                continue;
            }
            value = unit->hp * (Ov000_gUnitStats[unit->type].costDiv10 / 10);
            if (bestValue <= value) {
                bestX = tx;
                bestValue = value;
                bestY = ty;
                bestSlot = UNIT_SLOT_AT(tx, ty);
            }
        }
    }

    if (bestSlot == 0) {
        AiTargetList_Clear();
        return;
    }
    reduction = 0;
    reduction += DamageFromCannons(MAP_UNIT_OWNER(STATE, &STATE->units[bestSlot]), 0);
    AiTargetList_Add(bestX, bestY, bestSlot, damage - reduction);
}

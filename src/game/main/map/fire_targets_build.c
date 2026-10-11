#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int gFireTargetCount;

void FireTargets_Clear(void);
/* Defined with u16 parameters. Declared here with int x, y and slot because
 * this caller passes them without narrowing (needed to match). */
void FireTargets_Add(int x, int y, int unitSlot, u16 kind);
int Map_GetHitTargetKind(int x, int y);
void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16 *pos);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16 *pos);

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Player that owns a unit slot: 64 slots per player, from player 1 on. */
#define SLOT_PLAYER(slot) (STATE->players[((slot) >> 6) + 1])

/* Lists what the acting unit can fire on from its destination tile, on
 * every tile the range map reaches: enemy units (another team) that the
 * simulated battle lets it hit, and map objects it can damage. Returns the
 * count. */
int FireTargets_Build(void)
{
    int unitSlot;
    int x;
    int y;
    int kind;

    FireTargets_Clear();
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            unitSlot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
            kind = Map_GetHitTargetKind(x, y);
            if (unitSlot != 0) {
                if (SLOT_PLAYER(STATE->actingUnitIndex).team != SLOT_PLAYER(unitSlot).team) {
                    Combat_SimulateChecked(STATE->actingUnitIndex, unitSlot, &STATE->actingUnitDestX);
                    if (STATE->combatants[0].weapon != COMBAT_WEAPON_NONE) {
                        FireTargets_Add(x, y, unitSlot, 0);
                    }
                }
            } else if (kind != 0) {
                Combat_CalcDamageToTarget(STATE->actingUnitIndex, kind, &STATE->actingUnitDestX);
                if (STATE->combatants[0].weapon != COMBAT_WEAPON_NONE) {
                    FireTargets_Add(x, y, 0, kind);
                }
            }
        }
    }
    return gFireTargetCount;
}

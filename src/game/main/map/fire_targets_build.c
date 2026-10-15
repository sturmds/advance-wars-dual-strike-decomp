#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"
#include "game/ai_reach_tile.h"
#include "game/unit_target_entry.h"

int gFireTargetCount;
int data_022a6dd4;
UnitTargetEntry* data_022a6dcc;
AiReachTile* data_022a6dd0;
UnitTargetEntry data_022a6dd8[TARGET_LIST_ENTRIES];

void FireTargets_Clear(void);
/* Defined with u16 parameters. Declared here with int x, y and slot because
 * this caller passes them without narrowing (needed to match). */
void FireTargets_Add(int x, int y, int unitSlot, u16 kind);
int Map_GetHitTargetKind(int x, int y);
void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16* pos);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);

/* Player that owns a unit slot: 64 slots per player, from player 1 on. */
#define SLOT_PLAYER(slot) (MAP_STATE->players[((slot) >> 6) + 1])

/* Lists what the acting unit can fire on from its destination tile, on
 * every tile the range map reaches: enemy units (another team) that the
 * simulated battle lets it hit, and map objects it can damage. Returns the
 * count. */
int FireTargets_Build(void) {
  int unitSlot;
  int x;
  int y;
  int kind;

  FireTargets_Clear();
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      unitSlot =
          MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
      kind = Map_GetHitTargetKind(x, y);
      if (unitSlot != 0) {
        if (SLOT_PLAYER(MAP_STATE->actingUnitIndex).team !=
            SLOT_PLAYER(unitSlot).team) {
          Combat_SimulateChecked(MAP_STATE->actingUnitIndex, unitSlot,
                                 &MAP_STATE->actingUnitDestX);
          if (MAP_STATE->combatants[0].weapon != COMBAT_WEAPON_NONE) {
            FireTargets_Add(x, y, unitSlot, 0);
          }
        }
      } else if (kind != 0) {
        Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, kind,
                                  &MAP_STATE->actingUnitDestX);
        if (MAP_STATE->combatants[0].weapon != COMBAT_WEAPON_NONE) {
          FireTargets_Add(x, y, 0, kind);
        }
      }
    }
  }
  return gFireTargetCount;
}

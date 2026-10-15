#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int abs(int x);
int Map_GetHitTargetKind(
    int x,
    int y); /* guess: damage chart target kind of a map object or tile with hp at (x, y); 0: none */
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16* pos);

/* Writes mark into the range map on each tile within range (Manhattan
 * distance) of (x, y) that the acting unit, standing at actingUnitDestX/Y,
 * can attack: a map object or tile with hp, or a unit of another team. A
 * target counts when the battle set up for it picks a weapon. */
void RangeMap_MarkAttackTargets(int x, int y, s16 range, s8 mark) {
  s16 dy;
  s16 tx;
  s16 ty;
  Unk_02080440_48* state;
  int targetKind;
  u8 slot;

  for (dy = -range, ty = y - range; ty <= y + range; ty++, dy++) {
    if (ty < 0 || ty >= MAP_STATE->unk_1c4->mapHeight) {
      continue;
    }
    for (tx = x - range + abs(dy); tx <= x + range - abs(dy); tx++) {
      if (tx < 0 || tx >= MAP_STATE->unk_1c4->mapWidth) {
        continue;
      }
      targetKind = Map_GetHitTargetKind(tx, ty);
      if (targetKind != 0) {
        Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, targetKind,
                                  &MAP_STATE->actingUnitDestX);
        if (MAP_STATE->combatants[0].weapon != 0) {
          MAP_STATE->rangeMap[ty][tx] = mark;
        }
      } else {
        state = MAP_STATE;
        slot = state->mapTiles->unitAt[tx + state->mapTiles->rowStart[ty]];
        if (slot != 0 &&
            state->players[(state->actingUnitIndex >> 6) + 1].team !=
                state->players[(slot >> 6) + 1].team) {
          Combat_SimulateChecked(state->actingUnitIndex, slot,
                                 &state->actingUnitDestX);
          if (MAP_STATE->combatants[0].weapon != 0) {
            MAP_STATE->rangeMap[ty][tx] = mark;
          }
        }
      }
    }
  }
}

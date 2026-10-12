#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
s32 HwDiv32(s32 numerator, s32 denominator);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Looser form of AiFoot_JoinCapturingUnit, run by AiFoot_MoveAndAttack: joins
 * a same-type own unit that is capturing and shows under 10 hp, when the two
 * shown hp values add up to 15 or less and at least one of them is under 7.
 * There is no terrain test. */

void AiFoot_JoinCapturingUnitLoose(void) {
  MapUnit* unit;
  int slot;
  int x;
  int y;
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
      if (targetHp >= 7 && ownHp >= 7) {
        continue;
      }
      if (targetHp + ownHp <= 15) {
        AiUnit_PerformAction(x, y, AI_ACTION_JOIN, 0, 0);
      }
    }
  }
}

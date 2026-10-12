#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);
int AiActingUnit_GetSearchMovement(void);
void AiActingUnit_MarkAttackRange(int movement);
BOOL AiUnit_FindFiringTile(s16* tile);
BOOL Ai_HasEnemyWithinMoves(int x, int y, int arg2, int arg3);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)

#define NO_TILE 0x7fff

/* Finds a capture site (an entry of the unk_6088 table) for the acting
 * unit: the reachable tile with the lowest range map value that
 * Map_IsCapturableByActingUnit accepts, that Ai_HasEnemyWithinMoves accepts, and
 * whose site counter is under the AI profile's limit. The counter is
 * unk_04 for a unit that is not indirect and unk_05 for an indirect one.
 * Passes the tile to AiUnit_FindFiringTile and writes the result to out. */
BOOL Ai_FindCaptureSiteTile(s16* out) {
  int counterOffset;
  int best;
  int x;
  int y;
  s16 tile[2];

  best = NO_TILE;
  counterOffset =
      UnitType_GetAttackKind(STATE->actingUnit->type) == ATTACK_KIND_INDIRECT
          ? 2
          : 1;
  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Map_IsCapturableByActingUnit(x, y)) {
        continue;
      }
      if ((&STATE->unk_6088[MAP->unk_1928[x + MAP->rowStart[y]]]
                .claimCount)[counterOffset] >= gAiProfile->unk_08[5]) {
        continue;
      }
      if (best < STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Ai_HasEnemyWithinMoves(x, y, 1, 5)) {
        continue;
      }
      tile[0] = x;
      tile[1] = y;
      best = STATE->rangeMap[y][x];
    }
  }
  if (best == NO_TILE) {
    return FALSE;
  }
  if (!AiUnit_FindFiringTile(tile)) {
    return FALSE;
  }
  out[0] = tile[0];
  out[1] = tile[1];
  return TRUE;
}

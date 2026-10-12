#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiTurn_IsPhase3(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define TILE(x, y) (MAP->tiles[TILE_INDEX(x, y)])

/* An AI profile power hook (AiProfile.unk_00, see AiPlayer_TryUsePower).
 * Agrees to any power while gAiTurnState->factoryDomainsOwn is nonzero.
 * Otherwise it refuses a CO Power (powerKind 1), and agrees to a Super CO
 * Power when AiTurn_IsPhase3 does or the current player owns a city with no
 * unit on it. */
BOOL AiPowerCheck_HasFactoryOrFreeCity(int powerKind) {
  int x;
  int y;

  if (gAiTurnState->factoryDomainsOwn != 0) {
    return TRUE;
  }
  if (powerKind == 1) {
    return FALSE;
  }
  if (AiTurn_IsPhase3() == 1) {
    return TRUE;
  }
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_CITY &&
          STATE->unk_04 == (TILE(x, y) & 0xe0) &&
          MAP->unitAt[TILE_INDEX(x, y)] == 0) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

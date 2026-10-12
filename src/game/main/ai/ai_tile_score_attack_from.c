#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

s32 HwDiv32(s32 numerator, s32 denominator);
int Unit_GetTerrainStars(int player, int unitType,
                         int terrain); /* terrain defence stars */
int TerrainFirepower(int playerIndex, int unitType, int x, int y);
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)

/* Score terms, added to 15 - rangeMap value (0 to 15). Threat counts most:
 * (768 - threat) << 9. Other terms are the terrain stars term and these. */
#define SCORE_FIREPOWER_TILE \
  0x100 /* armed unit, and TerrainFirepower gives a bonus here */
#define SCORE_CAN_REPAIR \
  0x80000 /* not a factory tile, and the unit is repaired here */
#define SCORE_MARKED_TILE \
  0x100000 /* Unit_IsBlackHoleOnFlaggedTile: a Black Hole unit on a tile with bit 7 of tileHp set */
#define SCORE_LOW_THREAT 0x200000 /* threat under 20 */

/* How good (x, y) is as a tile for the AI's acting unit to attack from.
 * -1: off the map, occupied, or out of reach (rangeMap above
 * gAiTurnState->unk_05, or negative). Otherwise higher is better. Threat
 * is MapTiles.threatMax + 2 * threatSum. Terrain stars count unless the
 * tile is a factory tile (base, airport, port) the unit's team owns. */
int AiTile_ScoreAttackFrom(int x, int y) {
  int reach;
  int score;
  int terrain;
  int threat;
  int tile;

  if (x < 0 || y < 0 || x >= STATE->unk_1c4->mapWidth ||
      y >= STATE->unk_1c4->mapHeight) {
    return -1;
  }
  tile = x + MAP->rowStart[y];
  if (MAP->unitAt[tile] != 0) {
    return -1;
  }
  reach = STATE->rangeMap[y][x];
  if (reach < 0 || reach > gAiTurnState->unk_05) {
    return -1;
  }
  threat = MAP->threatMax[tile] + MAP->threatSum[tile] * 2;
  terrain = MAP_TILE_TERRAIN(MAP->tiles[tile]);
  score = 15 - reach;
  if (score < 0) {
    score = 0;
  }
  if (gTerrainFactoryKind[terrain] == 0 ||
      Unit_IsTileOwnedByTeam(STATE->actingUnitIndex, MAP->tiles[tile]) == 0) {
    score += (HwDiv32(Unit_GetTerrainStars(STATE->currentPlayer,
                                           STATE->actingUnit->type, terrain),
                      10) +
              1)
             << 4;
  }
  if (TerrainFirepower(STATE->currentPlayer, STATE->actingUnit->type, x, y) !=
      0) {
    if (Ov000_gUnitStats[STATE->actingUnit->type].minRange != 0) {
      score += SCORE_FIREPOWER_TILE;
    }
  }
  score += (768 - threat) << 9;
  if (gTerrainFactoryKind[terrain] == 0) {
    if (Unit_CanRepairAt(STATE->actingUnit, x, y) == 1) {
      score += SCORE_CAN_REPAIR;
    }
  }
  if (Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == 1) {
    score += SCORE_MARKED_TILE;
  }
  if (threat < 20) {
    score += SCORE_LOW_THREAT;
  }
  return score;
}

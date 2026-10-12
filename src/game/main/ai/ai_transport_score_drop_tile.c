#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

s32 HwDiv32(s32 numerator, s32 denominator);
int Unit_GetTerrainStars(int player, int unitType,
                         int terrain); /* terrain defence stars */
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

#define SCORE_CAN_REPAIR \
  0x10000 /* not a factory tile, and the unit is repaired here */
#define SCORE_MARKED_TILE \
  0x20000 /* Unit_IsBlackHoleOnFlaggedTile holds for the transport */

/* How good (x, y) is as a tile to drop cargo unit `unit` on, for
 * AiTransport_FindDropTiles. -1: off the map, occupied, or terrain the
 * unit cannot enter. Otherwise higher is better: low threatSum counts
 * most, then a repair tile and a tile Unit_IsBlackHoleOnFlaggedTile
 * accepts add fixed bonuses, and terrain stars count unless the tile is a
 * factory tile the acting unit's team owns. The stars are read for
 * Infantry whatever the unit type. */
int AiTransport_ScoreDropTile(int x, int y, MapUnit* unit) {
  /* declaration order needed to match (register allocation) */
  int score;
  int tile;
  int threat;
  int terrain;
  int factory;

  score = 0;
  if (x < 0 || y < 0 || x >= STATE->unk_1c4->mapWidth ||
      y >= STATE->unk_1c4->mapHeight) {
    return -1;
  }
  tile = x + MAP->rowStart[y];
  if (MAP->unitAt[tile] != 0) {
    return -1;
  }
  terrain = MAP_TILE_TERRAIN(MAP->tiles[tile]);
  if (CHART[Ov000_gUnitStats[unit->type].moveType][terrain] < 0) {
    return -1;
  }
  factory = gTerrainFactoryKind[terrain];
  threat = MAP->threatSum[tile];
  if (factory == 0 ||
      Unit_IsTileOwnedByTeam(STATE->actingUnitIndex, MAP->tiles[tile]) == 0) {
    score += (HwDiv32(Unit_GetTerrainStars(STATE->currentPlayer, UNIT_INFANTRY,
                                           terrain),
                      10) +
              1)
             << 4;
  }
  score += (256 - threat) << 8;
  if (gTerrainFactoryKind[terrain] == 0) {
    if (Unit_CanRepairAt(unit, x, y) == 1) {
      score += SCORE_CAN_REPAIR;
    }
  }
  if (Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == 1) {
    score += SCORE_MARKED_TILE;
  }
  return score;
}

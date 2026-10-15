#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])
#define PASSABLE(moveType) (CHART[moveType][terrain] != -1)
#define INFANTRY_MOVE_TYPE Ov000_gUnitStats[UNIT_INFANTRY].moveType
#define OWN_MOVE_TYPE Ov000_gUnitStats[unitType].moveType

/* Whether a transport of this type can use the tile. An APC or T-Copter needs
 * a tile Infantry can enter. A Lander or Black Boat needs one that Infantry
 * and the boat can both enter (a shore). A Cruiser or Carrier needs one it
 * can enter itself. Other types: FALSE. The name is a guess from the callers. */
BOOL Ai_IsTransportStopTile(int unitType, int x, int y) {
  int terrain =
      MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]);

  switch (unitType) {
    case UNIT_APC:
    case UNIT_T_COPTER:
      return PASSABLE(INFANTRY_MOVE_TYPE);
    case UNIT_BLACK_BOAT:
    case UNIT_LANDER:
      if (PASSABLE(INFANTRY_MOVE_TYPE) == FALSE) {
        return FALSE;
      }
      return PASSABLE(OWN_MOVE_TYPE);
    case UNIT_CRUISER:
    case UNIT_CARRIER:
      return PASSABLE(OWN_MOVE_TYPE);
  }
  return FALSE;
}

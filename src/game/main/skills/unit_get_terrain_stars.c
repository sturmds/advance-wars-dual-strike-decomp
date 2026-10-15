#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

BOOL Player_IsInGame(u8 player);

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* Terrain defense, in percent (10 per star), a unit of the player would get
 * on a terrain id: the terrain's stars minus the star cut
 * of each enemy front CO's day-to-day block (not its current power), never
 * below 0. Air units get 0. unitType 0 skips the air check. */
int Unit_GetTerrainStars(int player, int unitType, int terrain) {
  int defense;
  int enemy;

  if (unitType != UNIT_NONE &&
      Ov000_gUnitStats[unitType].moveGroup == MOVE_GROUP_AIR) {
    return 0;
  }
  defense = Ov000_gTerrainStats[terrain].defenseStars * 10;
  for (enemy = 1; enemy < ARMY_SLOT_COUNT; enemy++) {
    if (Player_IsInGame(enemy) && PLAYERS[player].team != PLAYERS[enemy].team) {
      defense -= gCoTable[PLAYERS[enemy].front.id & PLAYER_CO_ID_MASK]
                     .blocks[0]
                     .terrainStarCut *
                 10;
    }
  }
  if (defense < 0) {
    defense = 0;
  }
  return defense;
}

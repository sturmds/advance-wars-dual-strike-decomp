#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/skills.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Terrain defense, in percent (10 per star), that a unit of a player gets
 * on a terrain id. Air units get none. The player's terrain adjustment is
 * added, a negative total counts as 0, and Lash's Super Power doubles it.
 * unitType 0 skips the air check. */
int DoubleTerrainStars(int playerIndex, int unitType, int terrain) {
  Player* player;
  int defense;

  if (unitType != UNIT_NONE &&
      Ov000_gUnitStats[unitType].moveGroup == MOVE_GROUP_AIR) {
    return 0;
  }
  player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];
  defense =
      Ov000_gTerrainStats[terrain].defenseStars * 10 + player->terrainDefense;
  if (defense < 0) {
    return 0;
  }
  if (CheckSkillsEquipped(player->skills[0], SKILL_DOUBLE_TERRAIN_STARS)) {
    defense += defense;
  }
  return defense;
}

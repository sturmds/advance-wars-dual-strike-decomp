#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/skills.h"
#include "game/terrain.h"

int Player_GetCoTerrainFirepower(int playerIndex, int terrain);
int DoubleTerrainStars(int playerIndex, int unitType, int terrain);

#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* Firepower bonus a unit of a player gets from the tile it stands on:
 * the front CO's per-terrain value, half the terrain defense with Lash's
 * day-to-day skill, and +10 from the Force Rank that matches the
 * terrain. */
int TerrainFirepower(int playerIndex, int unitType, int x, int y) {
  MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  int terrain = MAP_TILE_TERRAIN(map->tiles[map->rowStart[y] + x]);
  int bonus = 0; /* needed to match: start at 0, then add */

  bonus += Player_GetCoTerrainFirepower(playerIndex, terrain);

  if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                          SKILL_TERRAIN_FIREPOWER)) {
    bonus += DoubleTerrainStars(playerIndex, unitType, terrain) >> 1;
  }

  switch (terrain) {
    case TERRAIN_SEA:
      if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                              SKILL_RANK_SEAMANSHIP)) {
        bonus += 10;
      }
      break;
    case TERRAIN_ROAD:
      if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                              SKILL_RANK_ROAD_RAGE)) {
        bonus += 10;
      }
      break;
    case TERRAIN_CITY:
      if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                              SKILL_RANK_URBAN_FIGHTER)) {
        bonus += 10;
      }
      break;
    case TERRAIN_MOUNTAIN:
      if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                              SKILL_RANK_MOUNTAINEER)) {
        bonus += 10;
      }
      break;
    case TERRAIN_WOOD:
      if (CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                              SKILL_RANK_RANGER)) {
        bonus += 10;
      }
      break;
  }
  return bonus;
}

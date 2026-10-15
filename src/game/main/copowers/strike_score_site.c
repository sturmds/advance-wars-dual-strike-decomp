#include "types.h"
#include "game/Unk_02080440.h"
#include "game/strike_target.h"
#include "game/unit_stats.h"

/* Nonzero when tile (x, y) is visible to the current player under fog. */
int Map_IsTileVisible(int x, int y);

/* Scores a strike centred on (x, y) for playerSlot: gStrikeUnitScore over
 * every unit within radius tiles (Manhattan distance), summed apart for
 * own-team and enemy units, combined by gStrikeSiteScore. visibleOnly == 1
 * skips units the player cannot see. On tile (skipX, skipY) the unit in
 * Unk_02080440_48.actingUnit counts instead of the one on the map. */
int Strike_ScoreSite(int playerSlot, int x, int y, int radius, int damage,
                     int visibleOnly, int skipX, int skipY) {
  int ty;
  int maxY;
  int maxX;
  int enemyScore;
  int minX;
  MapUnit* unit;
  int ownScore;
  int tx;
  int minY;
  u8 centreId;
  int dist;

  enemyScore = 0;
  ownScore = 0;
  minX = x - radius;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - radius;
  if (minY < 0) {
    minY = 0;
  }
  maxX = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1;
  if (x + radius < maxX) {
    maxX = x + radius;
  }
  maxY = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1;
  if (y + radius < maxY) {
    maxY = y + radius;
  }

  for (ty = minY; ty <= maxY; ty++) {
    for (tx = minX; tx <= maxX; tx++) {
      dist = 0;
      if (x - tx > 0) {
        dist += x - tx;
      } else {
        dist += tx - x;
      }
      if (y - ty > 0) {
        dist += y - ty;
      } else {
        dist += ty - y;
      }
      if (dist < 0 || dist > radius) {
        continue;
      }
      if (tx == skipX && ty == skipY) {
        unit = data_027c027c->unk_48[data_027c0284]->actingUnit;
      } else {
        Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
        MapTiles* tiles = state->mapTiles;
        unit = &state->units[tiles->unk_508[tiles->rowStart[ty] + tx]];
      }
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (visibleOnly == 1 && Map_IsTileVisible(unit->x, unit->y) == 0) {
        continue;
      }
      {
        Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
        Player* players = state->players;
        if (players[playerSlot].team ==
            players[MAP_UNIT_OWNER(state, unit)].team) {
          ownScore += gStrikeUnitScore(unit, damage, 0);
        } else {
          enemyScore += gStrikeUnitScore(unit, damage, 1);
        }
      }
    }
  }

  /* One extra point when an enemy unit stands on the centre tile. The
     * owner slot comes from the unit id as in MAP_UNIT_OWNER. */
  {
    MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;
    centreId = map->unk_508[map->rowStart[y] + x];
    if (centreId != 0) {
      if (data_027c027c->unk_48[data_027c0284]->players[playerSlot].team !=
          data_027c027c->unk_48[data_027c0284]
              ->players[(centreId >> 6) + 1]
              .team) {
        enemyScore++;
      }
    }
  }
  return gStrikeSiteScore(ownScore, enemyScore);
}

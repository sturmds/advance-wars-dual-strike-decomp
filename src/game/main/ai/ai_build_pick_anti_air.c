#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_unit_action.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
BOOL UnitSlot_OwnsTile(int slot, int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* Picks Anti-Air or Cruiser (AI_ANTI_AIR_*) to answer enemy aircraft; NONE
 * when the current player has no sea domain. An enemy aircraft on a tile a
 * Cruiser can enter counts for Cruisers, any other for Anti-Air; aircraft on
 * an airport their owner holds do not count. Each Anti-Air or Cruiser of the
 * current team takes one off its side. The _MORE results: enemy planes and
 * B-Copters have more hp than the team's units that can hit planes. */
int AiBuild_PickAntiAir(void) {
  /* needed to match: separate locals for the enemy loop, declared last */
  int aaNeed;
  int cruiserNeed;
  int airHp;
  int player;
  int slot;
  MapUnit* unit;
  int enemySlot;
  MapUnit* enemy;

  aaNeed = 0;
  cruiserNeed = 0;
  airHp = 0;
  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return AI_ANTI_AIR_NONE;
  }
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &STATE->units[slot];
        if (unit->type == UNIT_NONE) {
          continue;
        }
        if (unit->type == UNIT_ANTI_AIR) {
          aaNeed--;
        }
        if (unit->type == UNIT_CRUISER) {
          cruiserNeed--;
        }
        if (Ov000_gUnitStats[unit->type].targetMask & UNIT_CATEGORY_PLANE) {
          airHp -= unit->hp;
        }
      }
    } else {
      for (enemySlot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           enemySlot < Ov000_gPlayerFirstUnitSlot[player] + 51; enemySlot++) {
        enemy = &STATE->units[enemySlot];
        if (enemy->type == UNIT_NONE) {
          continue;
        }
        if (!(Ov000_gUnitStats[enemy->type].moveGroup & MOVE_GROUP_AIR)) {
          continue;
        }
        if (MAP_TILE_TERRAIN(TILE(enemy->x, enemy->y)) == TERRAIN_AIRPORT &&
            UnitSlot_OwnsTile((s16)enemySlot, enemy->x, enemy->y) == 1) {
          continue;
        }
        if (CHART[Ov000_gUnitStats[UNIT_CRUISER].moveType]
                 [MAP_TILE_TERRAIN(TILE(enemy->x, enemy->y))] > 0) {
          cruiserNeed++;
        } else {
          aaNeed++;
        }
        if (enemy->type != UNIT_T_COPTER &&
            (Ov000_gUnitStats[enemy->type].categoryMask &
             (UNIT_CATEGORY_PLANE | UNIT_CATEGORY_COPTER))) {
          airHp += enemy->hp;
        }
      }
    }
  }
  if (aaNeed < 0 && cruiserNeed < 0) {
    return AI_ANTI_AIR_NONE;
  }
  if (aaNeed > cruiserNeed) {
    return airHp > 0 ? AI_ANTI_AIR_AA_MORE : AI_ANTI_AIR_AA;
  }
  if (aaNeed < cruiserNeed) {
    return airHp > 0 ? AI_ANTI_AIR_CRUISER_MORE : AI_ANTI_AIR_CRUISER;
  }
  return AI_ANTI_AIR_NONE;
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/player.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/ai/ai_power_check.h"
#include "game/ai/ai_power.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */
/* a property other than a missile silo */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);

BOOL Terrain_IsPropertyNotSilo(u8 terrain);

/* TRUE for mode 1 when the current player has a factory, and for mode 2 when
 * AiTurn_IsPhase3 says so. Nothing calls it. */
BOOL func_020994b8(int mode) {
  if (mode == 1 && gAiTurnState->factoryDomainsOwn != 0) {
    return TRUE;
  }
  if (mode == 2 && AiTurn_IsPhase3() == 1) {
    return TRUE;
  }
  return FALSE;
}

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
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) == TERRAIN_CITY &&
          MAP_STATE->unk_04 == (MAP_TILE_AT(x, y) & 0xe0) &&
          MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] == 0) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* When func_02099528 says so, sets the AI phase to 3 and returns TRUE.
 * Nothing calls it. */
BOOL func_02099338(void) {
  if (func_02099528() == 1) {
    gAiTurnState->phase = 3;
    return TRUE;
  }
  return FALSE;
}

/* TRUE with fog of war or rain on the map. Otherwise FALSE for mode 2 in AI
 * phase 3, TRUE for the rest. Nothing calls it. */
BOOL func_020992c8(int mode) {
  Unk_02080440_1c4* rules = MAP_STATE->unk_1c4;

  if (rules->fog != 0 || rules->weather == WEATHER_RAIN) {
    return TRUE;
  }
  if (mode == 2 && gAiTurnState->phase == 3) {
    return FALSE;
  }
  return TRUE;
}

/* When AiTurn_IsPhase3 says so: TRUE if the current player has a unit on the
 * map (not carried) with less than 91 hp. Nothing calls it. */
BOOL func_020991f4(void) {
  int i;
  MapUnit* unit;

  if (AiTurn_IsPhase3() == 1) {
    for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
         i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
      unit = &MAP_STATE->units[i];
      if (unit->type != UNIT_NONE && !(unit->flags & MAP_UNIT_CARRIED) &&
          unit->hp < 91) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* TRUE when the weather is not snow; in snow, TRUE only for mode 2. Nothing
 * calls it. */
BOOL func_020991b0(int mode) {
  if (MAP_STATE->unk_1c4->weather != WEATHER_SNOW) {
    return TRUE;
  }
  return mode == 2;
}

/* TRUE when mode is 2 and AiTurn_IsPhase3 says so, or when the current player
 * has 30000 funds or more and a player on another team still in the game has
 * more front CO power charge. Nothing calls it. */
BOOL func_020990c4(int mode) {
  int player;

  if (mode == 2 && AiTurn_IsPhase3() == 1) {
    return TRUE;
  }
  if ((u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds >= 30000) {
    for (player = 1; player < ARMY_SLOT_COUNT; player++) {
      if (!Player_IsInGame(player)) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[player].team) {
        continue;
      }
      if (MAP_STATE->players[player].front.charge >
          MAP_STATE->players[MAP_STATE->currentPlayer].front.charge) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* An AI profile power hook (AiProfile.unk_00, see AiPlayer_TryUsePower).
 * For a Super CO Power (powerKind 2) it agrees only when AiTurn_IsPhase3
 * does (gAiTurnState->phase is 3). For a CO Power it agrees when enemy units stand on properties:
 * at least 5 of them, or at least a fifth of all enemy units (slots 1-50,
 * not Oozium or carried units). */
BOOL AiPowerCheck_EnemiesOnProperties(int powerKind) {
  int total = 0;
  int onProperty = 0;
  int player;
  int i;
  MapUnit* unit;

  if (powerKind == 2) {
    if (AiTurn_IsPhase3() == 1) {
      return TRUE;
    }
  } else {
    for (player = 1; player < ARMY_SLOT_COUNT; player++) {
      if (Player_IsInGame(player) &&
          MAP_STATE->players[MAP_STATE->currentPlayer].team !=
              MAP_STATE->players[player].team) {
        for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
             i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
          unit = &MAP_STATE->units[i];
          if (unit->type != UNIT_NONE && unit->type != UNIT_OOZIUM &&
              !(unit->flags & MAP_UNIT_CARRIED)) {
            if (Terrain_IsPropertyNotSilo(MAP_TILE_TERRAIN(
                    MAP_STATE->mapTiles
                        ->tiles[MAP_TILE_INDEX(unit->x, unit->y)])) == TRUE) {
              onProperty++;
            }
            total++;
          }
        }
      }
    }
    if (onProperty == 0) {
      return FALSE;
    }
    if (onProperty * 5 >= total || onProperty >= 5) {
      return TRUE;
    }
  }
  return FALSE;
}

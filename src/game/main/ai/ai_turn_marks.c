#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai_profile.h"
#include "game/math.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/range_map.h"
#include "game/ai/ai_turn_marks.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* UnitStats.ammo and UnitStats.fuel of record 0; the original code indexes
 * these two addresses by type * sizeof(UnitStats). */
#define TYPE_AMMO(type) Ov000_gUnitStats[type].ammo
#define TYPE_FUEL(type) Ov000_gUnitStats[type].fuel

extern u8 gAiTransportsSentToUnit[];

BOOL Player_IsInGame(u8 player);
/* 1 + the properties the player owns (bases, cities, airports, ports, Com Towers). */
int Player_CountPropertiesPlusOne(int player);
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);

/* Floods the map on foot from each entry of the unk_6088 table (infantry
 * with 110 movement, so effectively unlimited), setting rangeScratch to 1 on
 * every tile reached. An entry on a tile already marked is skipped, so
 * propertyRegions counts the separate land regions that hold an entry. */
void AiTurn_MarkReachableTiles(void) {
  int count;
  int x;
  int y;
  Unk_02080440_6088* entry;

  count = 0;
  entry = MAP_STATE->unk_6088;
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(0);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  for (; entry->terrain != MAP_SITE_END; entry++) {
    if (MAP_STATE->mapTiles
            ->rangeScratch[entry->x +
                           MAP_STATE->mapTiles->rowStart[entry->y]] != 0) {
      continue;
    }
    RangeMap_FillMoves(entry->x, entry->y, UNIT_INFANTRY, 110, 0);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] >= 0) {
          MAP_STATE->mapTiles
              ->rangeScratch[x + MAP_STATE->mapTiles->rowStart[y]] = 1;
        }
      }
    }
    count++;
  }
  gAiTurnState->propertyRegions = count;
}

/* Totals Player_CountPropertiesPlusOne over the neutral player and every player in the game
 * on another team. The name is a guess from what it counts. */
void AiTurn_CountCaptureTargets(void) {
  int player;

  gAiTurnState->captureTargets = 0;
  gAiTurnState->captureTargets += Player_CountPropertiesPlusOne(0);
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      gAiTurnState->captureTargets += Player_CountPropertiesPlusOne(player);
    }
  }
}

/* Clears transportWanted and two unit bits of unknown meaning (unk_0c_5, unk_08_7)
 * on the units of every player still in the game. Slots 1 to 50 of each
 * player's 64 are visited; the name is a guess. */
void AiTurn_ClearUnitMarks(void) {
  int player;
  int slot;
  int first;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    first = Ov000_gPlayerFirstUnitSlot[player];
    for (slot = first + 1; slot < first + 51; slot++) {
      MapUnit* unit = &data_027c027c->unk_48[data_027c0284]->units[slot];

      if (unit->type != UNIT_NONE) {
        unit->transportWanted = 0;
        unit->unk_0c_5 = 0;
        unit->unk_08_7 = 0;
      }
    }
  }
}

void Ai_ClearTransportsSentToUnit(void) {
  int i;

  for (i = 0; i < MAP_UNITS_PER_PLAYER; i++) {
    gAiTransportsSentToUnit[i] = 0;
  }
}

/* Sets MapUnit.serviceWanted for a unit that has none: 2 (repairs) when its hp
 * is below the active profile's unk_04, 1 (supplies) when it has no ammo
 * (and uses ammo) or its fuel percent is below the profile's unk_03. */
void AiUnit_ChooseServiceWanted(MapUnit* unit) {
  if (unit->hp < gAiActiveUnitProfile->unk_04) {
    unit->serviceWanted = 2;
    return;
  }
  if (TYPE_AMMO(unit->type) != 0 && unit->ammo == 0) {
    unit->serviceWanted = 1;
    return;
  }
  if ((u8)HwDiv32(unit->fuel * 100, TYPE_FUEL(unit->type)) <
      gAiActiveUnitProfile->unk_03) {
    unit->serviceWanted = 1;
  }
}

/* Mode 1 of MapUnit.serviceWanted (the unit wants supplies): drops the mode
 * once the unit has ammo, or never needed any, and its fuel is over half
 * the tank. */
void AiUnit_ClearSupplyWanted(MapUnit* unit) {
  if (TYPE_AMMO(unit->type) != 0 && unit->ammo == 0) {
    return;
  }
  if ((TYPE_FUEL(unit->type) >> 1) < unit->fuel) {
    unit->serviceWanted = 0;
  }
}

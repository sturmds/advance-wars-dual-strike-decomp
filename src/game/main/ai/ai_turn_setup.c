#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/ai_profile.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/ai/ai_turn_setup.h"
#include "game/ai/ai_turn_marks.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_AI_SET \
  data_ov000_022dbd69[MAP_STATE->unk_1c4->mapId * sizeof(MapHeader)]
#define CURRENT_MAP ((Unk_02080440*)(&data_027c0288.unk_00)[data_027c0284])

extern u8 data_ov000_022dbd69[];
/* MapHeader.unk_41 of map 0 */
extern AiProfileSet data_02161b8c[];
extern AiProfileMods gAiProfileMods[];
extern u8 gTerrainFactoryKind[];

AiUnitProfile* gAiActiveUnitProfile;

int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile);
int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);
s32 Rng_Range(s32 min, s32 max);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
BOOL Map_IsTileOwnedByTeamOf(int player, int x, int y);
BOOL Player_IsInGame(u8 player);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
/* entry of the active unit's type */
void func_0208d3fc(MapUnit* unit);

/* Sets the active unit profile for each of the current player's units,
 * then runs the check for its serviceWanted mode (1: AiUnit_ClearSupplyWanted, 2:
 * func_0208d3fc, else AiUnit_ChooseServiceWanted), which may change the mode. */
void AiTurn_UpdateUnitModes(void) {
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type == UNIT_NONE) {
      continue;
    }
    gAiActiveUnitProfile = &gAiProfile->units[unit->type - 1];
    switch (unit->serviceWanted) {
      case 1:
        AiUnit_ClearSupplyWanted(unit);
        break;
      case 2:
        func_0208d3fc(unit);
        break;
      default:
        AiUnit_ChooseServiceWanted(unit);
        break;
    }
  }
}

/* Fills the AiTurnState domain fields (domainsAny: everyone, domainsOwn: the
 * current player, domainsEnemy: other teams). Factories (base, airport, port)
 * give ground, air and sea bits by tile ownership; then every unit of a
 * player in the game adds sea for ships and air for aircraft; ground
 * units add nothing. The names are guesses from what the bits record. */
void AiTurn_FindUnitDomains(void) {
  DomainBits domainBit = data_02161a64;
  int x;
  int y;
  int player;
  int slot;
  MapUnit* unit;

  gAiTurnState->domainsAny = 0;
  gAiTurnState->domainsOwn = 0;
  gAiTurnState->domainsEnemy = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (!Map_IsTileOwnedByTeamOf(MAP_STATE->currentPlayer, x, y)) {
        gAiTurnState->domainsEnemy |=
            domainBit
                .bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))]];
      }
      if (MAP_STATE->unk_04 == (MAP_TILE_AT(x, y) & 0xe0)) {
        gAiTurnState->domainsOwn |=
            domainBit
                .bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))]];
      }
      gAiTurnState->domainsAny |=
          domainBit
              .bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))]];
    }
  }
  gAiTurnState->factoryDomainsOwn = gAiTurnState->domainsOwn;
  gAiTurnState->factoryDomainsEnemy = gAiTurnState->domainsEnemy;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA) {
        gAiTurnState->domainsAny |= AI_DOMAIN_SEA;
        if (Unit_IsCurrentPlayers(slot) == 1) {
          gAiTurnState->domainsOwn |= AI_DOMAIN_SEA;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
          gAiTurnState->domainsEnemy |= AI_DOMAIN_SEA;
        }
      } else if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR) {
        gAiTurnState->domainsAny |= AI_DOMAIN_AIR;
        if (Unit_IsCurrentPlayers(slot) == 1) {
          gAiTurnState->domainsOwn |= AI_DOMAIN_AIR;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
          gAiTurnState->domainsEnemy |= AI_DOMAIN_AIR;
        }
      }
    }
  }
}

/* Returns base + spread * roll / 100 - spread / 2, kept within 1-255. */
int AiProfile_Randomise(u8 base, u8 spread, int roll) {
  int value = base + (HwDiv32(spread * roll, 100) - (spread >> 1));

  if (value < 1) {
    value = 1;
  }
  if (value > 255) {
    value = 255;
  }
  return value;
}

/* Fills the current player's AI profile from a template and a mods
 * entry. The template values given as base and spread are randomised
 * with one roll for the whole profile. */
void AiProfile_Build(AiProfile* profile, int modsId, int templateId) {
  AiProfileMods* mods = &gAiProfileMods[modsId];
  AiProfileTemplate* template = &gAiProfileTemplates[templateId];
  int roll;
  Player* player;
  int type;
  int i;
  u32 hasPartner; /* needed to match: as BOOL (int) the compare folds away */

  roll = Rng_Range(0, 100);
  player = &MAP_STATE->players[MAP_STATE->currentPlayer];
  if (player->aiBehaviour == AI_BEHAVIOUR_DEFENSE) {
    roll = 0;
  } else if (player->aiBehaviour == AI_BEHAVIOUR_ASSAULT) {
    roll = 100;
  } else if (player->tagPowerHalf == TAG_POWER_BACK) {
    roll = 100;
  }

  profile->unk_00 = template->unk_00;
  hasPartner = MAP_STATE->players[MAP_STATE->currentPlayer].back.id != 0;
  if (hasPartner == 1) {
    profile->unk_04 = template->unk_06;
    profile->unk_05 = template->unk_07;
  } else {
    profile->unk_04 = template->unk_04;
    profile->unk_05 = template->unk_05;
  }
  profile->unk_06 = template->unk_08;
  profile->unk_07 = template->unk_09;
  profile->unk_08[0] = mods->unk_00[0];
  profile->unk_08[1] = mods->unk_00[1];
  profile->unk_08[2] = mods->unk_00[2];
  profile->unk_08[3] = mods->unk_00[3];
  profile->unk_08[4] = mods->unk_00[4];
  profile->unk_08[5] = mods->unk_00[5];
  profile->unk_08[6] = mods->unk_00[6];
  profile->unk_08[7] = mods->unk_00[7];
  profile->unk_08[8] = mods->unk_00[8];
  profile->unk_08[9] = mods->unk_00[9];
  profile->unk_08[10] = mods->unk_00[10];
  profile->unk_08[11] = mods->unk_00[11];

  for (type = UNIT_INFANTRY; type < UNIT_TYPE_COUNT; type++) {
    profile->units[type - 1].unk_00 = mods->units[type - 1][0];
    profile->units[type - 1].unk_05 = mods->units[type - 1][1];
    profile->units[type - 1].threatMaxLimit =
        AiProfile_Randomise(template->units[type - 1].base1,
                            template->units[type - 1].spread1, roll);
    profile->units[type - 1].threatSumLimit =
        AiProfile_Randomise(template->units[type - 1].base2,
                            template->units[type - 1].spread2, roll);
    profile->units[type - 1].unk_03 = template->units[type - 1].unk_04;
    profile->units[type - 1].unk_04 = template->units[type - 1].unk_05;
    for (i = 0; i < AI_ORDER_WEIGHT_COUNT; i++) {
      profile->units[type - 1].orderWeights[i] =
          template->units[type - 1].orderWeights[i];
    }
  }
}

/* Builds gAiProfile for a player. The mods come from the map's set. The
 * template is the front CO's from the set when CO effects are on, the
 * CO's from set 1 when the current player is human in the other screen's
 * state (game mode 0), and otherwise the set's first. */
void AiProfile_BuildForPlayer(int player) {
  if (MAP_AI_SET != 0) {
    if (MAP_STATE->coEffectsOn != 0) {
      AiProfile_Build(gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
                      data_02161b8c[MAP_AI_SET]
                          .templateIds[MAP_STATE->players[player].front.id &
                                       PLAYER_CO_ID_MASK]);
      return;
    }
    if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN &&
        CURRENT_MAP->unk_48[data_027c0284]
                ->players[MAP_STATE->currentPlayer]
                .controller == PLAYER_CONTROLLER_HUMAN) {
      AiProfile_Build(
          gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
          data_02161b8c[1].templateIds[MAP_STATE->players[player].front.id &
                                       PLAYER_CO_ID_MASK]);
      return;
    }
    AiProfile_Build(gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
                    data_02161b8c[MAP_AI_SET].templateIds[0]);
    return;
  }
  AiProfile_Build(
      gAiProfile, data_02161b8c[1].modsId,
      data_02161b8c[1].templateIds[MAP_STATE->players[player].front.id &
                                   PLAYER_CO_ID_MASK]);
}

/* Rolls the threat limits of every unit type again from a template. A Defense
 * player gets the low end of each range (roll 0), any other the high end
 * (roll 100). modsId is not used. */
void AiProfile_RerollThreatLimits(AiProfile* profile, int modsId,
                                  int templateId) {
  AiProfileTemplate* template = &gAiProfileTemplates[templateId];
  int roll;
  int type;

  roll = 100;
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour ==
      AI_BEHAVIOUR_DEFENSE) {
    roll = 0;
  }
  for (type = UNIT_INFANTRY; type < UNIT_TYPE_COUNT; type++) {
    profile->units[type - 1].threatMaxLimit =
        AiProfile_Randomise(template->units[type - 1].base1,
                            template->units[type - 1].spread1, roll);
    profile->units[type - 1].threatSumLimit =
        AiProfile_Randomise(template->units[type - 1].base2,
                            template->units[type - 1].spread2, roll);
  }
}

/* Like AiProfile_BuildForPlayer, but always picks the template of the player's
 * front CO, and calls AiProfile_RerollThreatLimits, which rolls the unit values of the
 * template again (it takes a mods id it does not use). The AI calls it before
 * it uses a CO power. */
void AiProfile_RerandomiseForPlayer(int player) {
  if (MAP_AI_SET != 0) {
    AiProfile_RerollThreatLimits(
        gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
        data_02161b8c[MAP_AI_SET]
            .templateIds[MAP_STATE->players[player].front.id &
                         PLAYER_CO_ID_MASK]);
    return;
  }
  AiProfile_RerollThreatLimits(
      gAiProfile, data_02161b8c[1].modsId,
      data_02161b8c[1].templateIds[MAP_STATE->players[player].front.id &
                                   PLAYER_CO_ID_MASK]);
}

/* distance in tiles, Manhattan */
/* AI queue routine. For each unk_6088 entry whose tile the current player's
 * team does not own, floods the map with an infantry move range and updates
 * MapTiles.rangeScratch2 on the tiles reached: bit 7 set, bit 6 set when the
 * entry is within a Mech's move range (straight-line tile distance), and the
 * weight in bits 0-5 raised by 8 for an HQ, base, airport, port or Com
 * Tower and by 1 otherwise, up to 63. The names are guesses. */
void AiTurn_BuildCaptureMap(void) {
  Unk_02080440_6088* entry;
  int infantryRange;
  int weight;
  int mechRange;
  int x;
  int y;

  entry = MAP_STATE->unk_6088;
  infantryRange =
      Player_GetUnitMoveRange(MAP_STATE->currentPlayer, UNIT_INFANTRY);
  mechRange = Player_GetUnitMoveRange(MAP_STATE->currentPlayer, UNIT_MECH);
  TileMap_Fill(MAP_STATE->mapTiles->rangeScratch2, 0);
  /* needed to match: the pointer starts one entry back and moves first */
  entry--;
  for (;;) {
    if ((++entry)->terrain == 0xff) {
      break;
    }
    if (Unit_IsTileOwnedByTeam(
            (s16)MAP_STATE->currentUnitBase,
            MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(entry->x, entry->y)]) ==
        1) {
      continue;
    }
    RangeMap_FillMoves(entry->x, entry->y, UNIT_INFANTRY, infantryRange, 0);
    if (Terrain_IsHqBaseAirportPortOrTower(
            MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(entry->x, entry->y)]) ==
        1) {
      weight = 8;
    } else {
      weight = 1;
    }
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] |= 0x80;
        if (Map_GetDistance(entry->x, entry->y, x, y) <= mechRange) {
          MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] |= 0x40;
        }
        if (((u8)MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] &
             0x3f) +
                weight <
            63) {
          MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] += weight;
        } else {
          MAP_STATE->mapTiles->rangeScratch2[MAP_TILE_INDEX(x, y)] |= 0x3f;
        }
      }
    }
  }
}

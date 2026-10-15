#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/map_header.h"
#include "game/unlocks.h"

/* Whether `player` may build `unitType` on the current campaign mission (the
 * build menu asks only in GAME_MODE_CAMPAIGN). Black Hole armies build
 * everything. Missions 0-4 ban Medium Tanks, Neotanks, Rockets and Missiles.
 * Megatanks, Stealths and Carriers are allowed only on listed missions;
 * Piperunners, Black Bombs and Black Boats need a game flag each (99, 100,
 * 101; likely set by campaign progress). Oozium never. */
BOOL Campaign_CanPlayerBuildUnit(int player, int unitType) {
  u16 mapId;

  if (MAP_STATE->players[player].armyColour == ARMY_BLACK_HOLE) {
    return TRUE;
  }
  mapId = MAP_STATE->unk_1c4->mapId;
  if (mapId >= MAP_CAMPAIGN_FIRST && mapId <= MAP_CAMPAIGN_FIRST + 4) {
    switch (unitType) {
      case UNIT_MEDIUM_TANK:
      case UNIT_NEOTANK:
      case UNIT_ROCKETS:
      case UNIT_MISSILES:
        return FALSE;
    }
  }
  switch (unitType) {
    case UNIT_MEGATANK:
      switch (mapId) {
        case MAP_CAMPAIGN_FIRST + 13:
        case MAP_CAMPAIGN_FIRST + 14:
        case MAP_CAMPAIGN_FIRST + 15:
        case MAP_CAMPAIGN_FIRST + 16:
        case MAP_CAMPAIGN_FIRST + 17:
        case MAP_CAMPAIGN_FIRST + 18:
        case MAP_CAMPAIGN_FIRST + 19:
        case MAP_CAMPAIGN_FIRST + 20:
        case MAP_CAMPAIGN_FIRST + 21:
        case MAP_CAMPAIGN_FIRST + 22:
        case MAP_CAMPAIGN_FIRST + 23:
        case MAP_CAMPAIGN_FIRST + 24:
        case MAP_CAMPAIGN_FIRST + 27:
        case MAP_CAMPAIGN_FIRST + 30:
        case MAP_CAMPAIGN_FIRST + 31:
        case MAP_CAMPAIGN_FIRST + 32:
          return TRUE;
        default:
          return FALSE;
      }
    case UNIT_STEALTH:
    case UNIT_CARRIER:
      switch (mapId) {
        case MAP_CAMPAIGN_FIRST + 14:
        case MAP_CAMPAIGN_FIRST + 15:
        case MAP_CAMPAIGN_FIRST + 16:
        case MAP_CAMPAIGN_FIRST + 17:
        case MAP_CAMPAIGN_FIRST + 18:
        case MAP_CAMPAIGN_FIRST + 19:
        case MAP_CAMPAIGN_FIRST + 20:
        case MAP_CAMPAIGN_FIRST + 21:
        case MAP_CAMPAIGN_FIRST + 22:
        case MAP_CAMPAIGN_FIRST + 23:
        case MAP_CAMPAIGN_FIRST + 24:
        case MAP_CAMPAIGN_FIRST + 30:
        case MAP_CAMPAIGN_FIRST + 31:
        case MAP_CAMPAIGN_FIRST + 32:
          return TRUE;
        default:
          return FALSE;
      }
    case UNIT_BLACK_BOAT:
      return GameFlag_IsSet(101) ? TRUE : FALSE;
    case UNIT_BLACK_BOMB:
      return GameFlag_IsSet(100) ? TRUE : FALSE;
    case UNIT_PIPERUNNER:
      return GameFlag_IsSet(99) ? TRUE : FALSE;
    case UNIT_OOZIUM:
      return FALSE;
  }
  return TRUE;
}

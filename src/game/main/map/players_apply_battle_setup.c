#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"

u8 Map_GetLoadedPlayerCount(void);
void Player_SetCo(int slot, int which, int coId);
void Player_CountUnitBuilt(int player, int unitType);
void func_020c1440(void);

/* Starting power meter per slot, front and back CO: u16 [5][2] from 0xa6,
 * which the header splits into pad_a6 and unk_aa. */
#define START_CHARGE (((u16(*)[2])(MAP_STATE->unk_aa - 2)))

/* Battle start: copies the setup in the game state into players 1-4.
 * Slots past the map's player count lose their controller. Each player gets
 * controller, army colour (palette, from state->army), front and back CO
 * (none without a controller), starting power meter, team, army style
 * (sprite set: CoRecord.armyStyle + 1 of the front CO) and AI behaviour by
 * gameMode. Then every unit already on the map counts as built. */
void Players_ApplyBattleSetup(void) {
  int i; /* needed to match: one counter for the slot loop and the unit loop */
  int unused;

  for (unused = PLAYER_SLOT_LAST - Map_GetLoadedPlayerCount(); unused >= 1;
       unused--) {
    MAP_STATE->slotControllers[ARMY_SLOT_COUNT - unused] =
        PLAYER_CONTROLLER_NONE;
  }
  MAP_STATE->players[1].controller = MAP_STATE->slotControllers[1];
  MAP_STATE->players[2].controller = MAP_STATE->slotControllers[2];
  MAP_STATE->players[3].controller = MAP_STATE->slotControllers[3];
  MAP_STATE->players[4].controller = MAP_STATE->slotControllers[4];

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    MAP_STATE->players[i].armyColour = MAP_STATE->army[i];
    if (MAP_STATE->players[i].controller == PLAYER_CONTROLLER_NONE) {
      Player_SetCo(i, 0, 0);
      Player_SetCo(i, 1, 0);
    } else {
      Player_SetCo(i, 0, MAP_STATE->playerCos[i][0]);
      Player_SetCo(i, 1, MAP_STATE->playerCos[i][1]);
    }
    MAP_STATE->players[i].front.charge = START_CHARGE[i][0];
    MAP_STATE->players[i].back.charge = START_CHARGE[i][1];
    MAP_STATE->players[i].team = MAP_STATE->team[i];
    MAP_STATE->players[i].armyStyle =
        gCoTable[MAP_STATE->playerCos[i][0] & PLAYER_CO_ID_MASK].armyStyle + 1;
    switch (MAP_STATE->gameMode) {
      case 0:
        /* the last slot of the map */
        if (i - 1 == Map_GetLoadedPlayerCount() - 1) {
          MAP_STATE->players[i].aiBehaviour = AI_BEHAVIOUR_GENERAL;
        } else {
          MAP_STATE->players[i].aiBehaviour = gPlayRecords.campaignAiBehaviour;
        }
        break;
      case 1: /* War Room: slot 1 is the player */
        if (i != 1) {
          MAP_STATE->players[i].aiBehaviour = AI_BEHAVIOUR_GENERAL;
        } else {
          MAP_STATE->players[i].aiBehaviour = gPlayRecords.warRoomAiBehaviour;
        }
        break;
      default:
        MAP_STATE->players[i].aiBehaviour = MAP_STATE->unk_8c;
        break;
    }
  }

  func_020c1440();
  if (MAP_STATE->coEffectsOn == 0) {
    MAP_STATE->unk_5c = 0;
  }
  for (i = 1; i < MAP_UNIT_COUNT; i++) {
    if (MAP_STATE->units[i].type != 0) {
      Player_CountUnitBuilt((i >> 6) + 1, MAP_STATE->units[i].type);
    }
  }
}

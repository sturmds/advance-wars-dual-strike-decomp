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

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starting power meter per slot, front and back CO: u16 [5][2] from 0xa6,
 * which the header splits into pad_a6 and unk_aa. */
#define START_CHARGE (((u16(*)[2])(STATE->unk_aa - 2)))

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
    STATE->slotControllers[ARMY_SLOT_COUNT - unused] = PLAYER_CONTROLLER_NONE;
  }
  STATE->players[1].controller = STATE->slotControllers[1];
  STATE->players[2].controller = STATE->slotControllers[2];
  STATE->players[3].controller = STATE->slotControllers[3];
  STATE->players[4].controller = STATE->slotControllers[4];

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    STATE->players[i].armyColour = STATE->army[i];
    if (STATE->players[i].controller == PLAYER_CONTROLLER_NONE) {
      Player_SetCo(i, 0, 0);
      Player_SetCo(i, 1, 0);
    } else {
      Player_SetCo(i, 0, STATE->playerCos[i][0]);
      Player_SetCo(i, 1, STATE->playerCos[i][1]);
    }
    STATE->players[i].front.charge = START_CHARGE[i][0];
    STATE->players[i].back.charge = START_CHARGE[i][1];
    STATE->players[i].team = STATE->team[i];
    STATE->players[i].armyStyle =
        gCoTable[STATE->playerCos[i][0] & PLAYER_CO_ID_MASK].armyStyle + 1;
    switch (STATE->gameMode) {
      case 0:
        /* the last slot of the map */
        if (i - 1 == Map_GetLoadedPlayerCount() - 1) {
          STATE->players[i].aiBehaviour = AI_BEHAVIOUR_GENERAL;
        } else {
          STATE->players[i].aiBehaviour = gPlayRecords.campaignAiBehaviour;
        }
        break;
      case 1: /* War Room: slot 1 is the player */
        if (i != 1) {
          STATE->players[i].aiBehaviour = AI_BEHAVIOUR_GENERAL;
        } else {
          STATE->players[i].aiBehaviour = gPlayRecords.warRoomAiBehaviour;
        }
        break;
      default:
        STATE->players[i].aiBehaviour = STATE->unk_8c;
        break;
    }
  }

  func_020c1440();
  if (STATE->coEffectsOn == 0) {
    STATE->unk_5c = 0;
  }
  for (i = 1; i < MAP_UNIT_COUNT; i++) {
    if (STATE->units[i].type != 0) {
      Player_CountUnitBuilt((i >> 6) + 1, STATE->units[i].type);
    }
  }
}

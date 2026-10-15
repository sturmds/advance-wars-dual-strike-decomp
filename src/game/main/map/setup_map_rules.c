#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

int Map_GetPlayerCount(int mapId); /* MapHeader.playerCount for most map ids */
void SetMapWeather(int weather); /* sets unk_1c4 weather and default weather */
void MapRules_SetDefaults(void);
int Campaign_IsHardMode(void);

#define MAP_ID (MAP_STATE->unk_1c4->mapId)

/* Applies the current map's header to the rules: fog, weather, tileset,
 * the tag partner flags and who controls each slot (slotControllers), each by game
 * mode (gameMode). data_027c0288 holds two states; modes 0 and 1 switch to
 * one of them to write there, then switch back. */
void SetupMapRules(void) {
  MapRules_SetDefaults();
  switch (MAP_STATE->gameMode) {
    case 0: {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;
      int i;

      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      /* AI for the slot that equals the map's player count, else human */
      for (i = 1; i < ARMY_SLOT_COUNT; i++) {
        if (i == Map_GetPlayerCount(MAP_ID)) {
          MAP_STATE->slotControllers[i] = PLAYER_CONTROLLER_AI;
        } else {
          MAP_STATE->slotControllers[i] = PLAYER_CONTROLLER_HUMAN;
        }
      }
      if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
        MAP_STATE->unk_1c4->fog = 1;
      }
      MAP_STATE->timeLimitFrames = 0;
      MAP_STATE->dayLimit =
          Ov000_gMapHeaders[MAP_ID].unk_30[Campaign_IsHardMode()];
      MAP_STATE->propertyGoal =
          Ov000_gMapHeaders[MAP_ID].unk_34[Campaign_IsHardMode()];
      MAP_STATE->partnerBoostHuman = 1;
      MAP_STATE->partnerBoostAi = 1;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    }
    case 1:
      MAP_STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
      MAP_STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
      MAP_STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
      MAP_STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
      if (Ov000_gMapHeaders[MAP_ID].variantMapIds[0] != 0) {
        Unk_027c0280* saved0280;
        Unk_02080440* saved027c;

        if (MAP_STATE->unk_59 <= 1) {
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_04);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_04);
          MAP_STATE->slotControllers[1] = PLAYER_CONTROLLER_AI;
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
        } else {
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_04);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_04);
          MAP_STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
        }
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        MAP_STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
        MAP_STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
        MAP_STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
      }
      switch (MAP_STATE->unk_59) {
        case 0:
          MAP_STATE->partnerBoostHuman = 0;
          MAP_STATE->partnerBoostAi = 0;
          break;
        case 1:
          MAP_STATE->partnerBoostHuman = 1;
          MAP_STATE->partnerBoostAi = 1;
          break;
        case 2:
          MAP_STATE->partnerBoostHuman = 0;
          MAP_STATE->partnerBoostAi = 0;
          break;
        case 3:
          MAP_STATE->partnerBoostHuman = 1;
          MAP_STATE->partnerBoostAi = 1;
          break;
      }
      break;
    case 3:
      MAP_STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
      MAP_STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
      MAP_STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
      MAP_STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
      if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
        MAP_STATE->unk_1c4->fog = 1;
      }
      MAP_STATE->partnerBoostHuman = 1;
      MAP_STATE->partnerBoostAi = 1;
      break;
  }
  SetMapWeather(Ov000_gMapHeaders[MAP_ID].weather);
  MAP_STATE->unk_1c4->tileset = Ov000_gMapHeaders[MAP_ID].tileset;
}

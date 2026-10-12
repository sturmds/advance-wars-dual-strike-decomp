#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int Map_GetPlayerCount(int mapId); /* MapHeader.playerCount for most map ids */
void SetMapWeather(int weather); /* sets unk_1c4 weather and default weather */
void MapRules_SetDefaults(void);
int Campaign_IsHardMode(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

/* Shorthands for the current state and its map id; the original code
 * reloads both through the globals on every access. */
#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP_ID (STATE->unk_1c4->mapId)

/* Applies the current map's header to the rules: fog, weather, tileset,
 * the tag partner flags and who controls each slot (slotControllers), each by game
 * mode (gameMode). data_027c0288 holds two states; modes 0 and 1 switch to
 * one of them to write there, then switch back. */
void SetupMapRules(void) {
  MapRules_SetDefaults();
  switch (STATE->gameMode) {
    case 0: {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;
      int i;

      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      /* AI for the slot that equals the map's player count, else human */
      for (i = 1; i < 5; i++) {
        if (i == Map_GetPlayerCount(MAP_ID)) {
          STATE->slotControllers[i] = PLAYER_CONTROLLER_AI;
        } else {
          STATE->slotControllers[i] = PLAYER_CONTROLLER_HUMAN;
        }
      }
      if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
        STATE->unk_1c4->fog = 1;
      }
      STATE->timeLimitFrames = 0;
      STATE->dayLimit = Ov000_gMapHeaders[MAP_ID].unk_30[Campaign_IsHardMode()];
      STATE->propertyGoal =
          Ov000_gMapHeaders[MAP_ID].unk_34[Campaign_IsHardMode()];
      STATE->partnerBoostHuman = 1;
      STATE->partnerBoostAi = 1;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    }
    case 1:
      STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
      STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
      STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
      STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
      if (Ov000_gMapHeaders[MAP_ID].variantMapIds[0] != 0) {
        Unk_027c0280* saved0280;
        Unk_02080440* saved027c;

        if (STATE->unk_59 <= 1) {
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_04);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_04);
          STATE->slotControllers[1] = PLAYER_CONTROLLER_AI;
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
        } else {
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_04);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_04);
          STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
        }
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
        STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
        STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
      }
      switch (STATE->unk_59) {
        case 0:
          STATE->partnerBoostHuman = 0;
          STATE->partnerBoostAi = 0;
          break;
        case 1:
          STATE->partnerBoostHuman = 1;
          STATE->partnerBoostAi = 1;
          break;
        case 2:
          STATE->partnerBoostHuman = 0;
          STATE->partnerBoostAi = 0;
          break;
        case 3:
          STATE->partnerBoostHuman = 1;
          STATE->partnerBoostAi = 1;
          break;
      }
      break;
    case 3:
      STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
      STATE->slotControllers[2] = PLAYER_CONTROLLER_AI;
      STATE->slotControllers[3] = PLAYER_CONTROLLER_AI;
      STATE->slotControllers[4] = PLAYER_CONTROLLER_AI;
      if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
        STATE->unk_1c4->fog = 1;
      }
      STATE->partnerBoostHuman = 1;
      STATE->partnerBoostAi = 1;
      break;
  }
  SetMapWeather(Ov000_gMapHeaders[MAP_ID].weather);
  STATE->unk_1c4->tileset = Ov000_gMapHeaders[MAP_ID].tileset;
}

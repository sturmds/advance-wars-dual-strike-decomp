#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/play_records.h"
#include "game/screen_other_map.h"

void MapRules_SetDefaultTeams(void); /* team[1..4] = 0, 1, 2, 3 */
void Map_SetCoSelectPhase(u8 value);
void func_0200a630(void); /* data_027c0284 = 0 */

/* Resets the rules of the current state before SetupMapRules applies a
 * map: four human players in armies 1-4 on their own teams, default CO
 * pairs, 1000 funds per property, no fog and clear weather on both screen
 * states (data_027c0288), tag partner skills on for everyone. */
void MapRules_SetDefaults(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  MAP_STATE->unk_10 = 0;
  MAP_STATE->unk_14 = 0;
  MAP_STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
  MAP_STATE->slotControllers[2] = PLAYER_CONTROLLER_HUMAN;
  MAP_STATE->slotControllers[3] = PLAYER_CONTROLLER_HUMAN;
  MAP_STATE->slotControllers[4] = PLAYER_CONTROLLER_HUMAN;
  MAP_STATE->army[1] = 1;
  MAP_STATE->army[2] = 2;
  MAP_STATE->army[3] = 3;
  MAP_STATE->army[4] = 4;
  MAP_STATE->playerCos[1][0] = CO_JAKE;
  MAP_STATE->playerCos[2][0] = CO_SASHA;
  MAP_STATE->playerCos[3][0] = CO_JAVIER;
  MAP_STATE->playerCos[4][0] = CO_GRIMM;
  MAP_STATE->playerCos[1][1] = CO_RACHEL;
  MAP_STATE->playerCos[2][1] = CO_COLIN;
  MAP_STATE->playerCos[3][1] = CO_EAGLE;
  MAP_STATE->playerCos[4][1] = CO_SENSEI;
  MAP_STATE->unk_aa[0] = 0;
  MAP_STATE->unk_aa[1] = 0;
  MAP_STATE->unk_aa[2] = 0;
  MAP_STATE->unk_aa[3] = 0;
  MAP_STATE->unk_aa[4] = 0;
  MAP_STATE->unk_aa[5] = 0;
  MAP_STATE->unk_aa[6] = 0;
  MAP_STATE->unk_aa[7] = 0;
  MapRules_SetDefaultTeams();
  MAP_STATE->timeLimitFrames = 0;
  MAP_STATE->dayLimit = 0;
  MAP_STATE->propertyGoal = 0;
  MAP_STATE->fundsPerProperty = 1000;
  MAP_STATE->unk_74 = 0;
  MAP_STATE->unk_64 = 0;
  MAP_STATE->unk_68 = 0;
  MAP_STATE->unk_6c = 0;
  MAP_STATE->unk_70 = 0;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MAP_STATE->unk_1c4->fog = 0;
  MAP_STATE->unk_1c4->tileset = 0;
  MAP_STATE->unk_1c4->defaultWeather = 0;
  MAP_STATE->unk_1c4->weather = WEATHER_CLEAR;
  MAP_STATE->unk_1c4->randomWeather = 0;
  MAP_STATE->unk_1c4->nextWeather = 0;
  MAP_STATE->unk_1c4->unk_37 = 3;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  MAP_STATE->unk_1c4->fog = 0;
  MAP_STATE->unk_1c4->tileset = 0;
  MAP_STATE->unk_1c4->defaultWeather = 0;
  MAP_STATE->unk_1c4->weather = WEATHER_CLEAR;
  MAP_STATE->unk_1c4->randomWeather = 0;
  MAP_STATE->unk_1c4->nextWeather = 0;
  MAP_STATE->unk_1c4->unk_37 = 3;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  MAP_STATE->unk_84 = 1;
  MAP_STATE->luckOnRequestOnly = 0;
  MAP_STATE->unk_8c = 2;
  MAP_STATE->unk_86 = 0;
  MAP_STATE->secondMapId = 0;
  MAP_STATE->coEffectsOn = 1;
  MAP_STATE->unk_5c = 1;
  MAP_STATE->partnerBoostHuman = 1;
  MAP_STATE->partnerBoostAi = 1;
  MAP_STATE->visualMode = gPlayRecords.visualMode;
  MAP_STATE->musicReady = gPlayRecords.unk_0e;
  Map_SetCoSelectPhase(0);
  func_0200a630();
}

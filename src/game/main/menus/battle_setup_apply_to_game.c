#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/play_records.h"
#include "game/weather.h"
#include "game/screen_other_map.h"

void Co_SetForceRankSetIndex(int co, int player, int set);
int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
int Link_GetPlayerOfSlot(int slot);

extern u32 gLinkPlay;

/* data_027c0288 holds two game states, one per map; see Map_InitBattle. */
#define SECOND_MAP (data_027c0288.unk_04)

/* Controller of a slot in link play: the local player keeps the menu's
 * choice, AI stays AI, and other humans become 3 + their link player. */
static inline void SetSlotController(int i, int map) {
  if (Link_IsActive() == 1) {
    if (i == Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
      MAP_STATE->slotControllers[i + 1] = gMenuState.setup.controllers[i][map];
    } else if (gMenuState.setup.controllers[i][map] != PLAYER_CONTROLLER_AI) {
      MAP_STATE->slotControllers[i + 1] = Link_GetPlayerOfSlot(i + 1) + 3;
    } else {
      MAP_STATE->slotControllers[i + 1] = PLAYER_CONTROLLER_AI;
    }
  } else {
    MAP_STATE->slotControllers[i + 1] = gMenuState.setup.controllers[i][map];
  }
}

/* A macro, not an inline function: needed to match, since with arguments
 * the compiler merges the switch cases. */
#define SET_WEATHER(random, value)                \
  do {                                            \
    MAP_STATE->unk_1c4->randomWeather = (random); \
    MAP_STATE->unk_1c4->weather = (value);        \
    MAP_STATE->unk_1c4->nextWeather = (value);    \
    MAP_STATE->unk_1c4->defaultWeather = (value); \
  } while (0)

/* Menu weather setting 0 is random weather; 1-4 are clear, rain, snow
 * and sand. */
static inline void ApplyWeatherSetting(void) {
  switch (gMenuState.setup.unk_2dc) {
    case 0:
      SET_WEATHER(1, WEATHER_CLEAR);
      break;
    case 1:
    default:
      SET_WEATHER(0, WEATHER_CLEAR);
      break;
    case 2:
      SET_WEATHER(0, WEATHER_RAIN);
      break;
    case 3:
      SET_WEATHER(0, WEATHER_SNOW);
      break;
    case 4:
      SET_WEATHER(0, WEATHER_SAND);
      break;
  }
}

/* Copies the battle setup state (gMenuState) into the current game:
 * the rules, then CO, Force Rank set, controller, army colour and team of
 * players 1-4, then the weather. In a two-map battle the map rules,
 * controllers and weather are also written to the second map's state.
 * The reverse of BattleSetup_LoadFromGame. */
void BattleSetup_ApplyToGame(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  MAP_STATE->visualMode = gMenuState.setup.visualMode;
  MAP_STATE->unk_1c4->fog = gMenuState.setup.fog;
  MAP_STATE->unk_1c4->tileset = gMenuState.setup.unk_2dd;
  MAP_STATE->dayLimit = gMenuState.setup.unk_2de;
  MAP_STATE->fundsPerProperty = gMenuState.setup.fundsPerProperty;
  MAP_STATE->coEffectsOn = gMenuState.setup.coEffectsOn;
  MAP_STATE->propertyGoal = gMenuState.setup.unk_2df;
  MAP_STATE->unk_8c = gMenuState.setup.unk_2e2;
  if (gLinkPlay == 0) {
    gPlayRecords.unk_15 = gMenuState.setup.unk_2e2;
  }

  if (MAP_STATE->twoMaps == 1) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(SECOND_MAP);
    saved027c = data_027c027c;
    Screen_SetCurrent(SECOND_MAP);
    MAP_STATE->unk_1c4->fog = gMenuState.setup.fog;
    MAP_STATE->unk_1c4->tileset = gMenuState.setup.unk_2dd;
    MAP_STATE->dayLimit = gMenuState.setup.unk_2de;
    MAP_STATE->fundsPerProperty = gMenuState.setup.fundsPerProperty;
    MAP_STATE->coEffectsOn = gMenuState.setup.coEffectsOn;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.setup.cos); i++) {
    MAP_STATE->playerCos[i + 1][0] =
        gMenuState.setup.cos[i][0] & PLAYER_CO_ID_MASK;
    MAP_STATE->playerCos[i + 1][1] =
        gMenuState.setup.cos[i][1] & PLAYER_CO_ID_MASK;
    Co_SetForceRankSetIndex(gMenuState.setup.cos[i][0], i + 1,
                            gMenuState.setup.forceRankSets[i][0]);
    Co_SetForceRankSetIndex(gMenuState.setup.cos[i][1], i + 1,
                            gMenuState.setup.forceRankSets[i][1]);
    SetSlotController(i, 0);
    if (i >= gMenuState.setup.unk_2e3) {
      MAP_STATE->slotControllers[i + 1] = PLAYER_CONTROLLER_NONE;
    }
    MAP_STATE->army[i + 1] = gMenuState.setup.armyColours[i];
    MAP_STATE->team[i + 1] = gMenuState.setup.teams[i];

    if (MAP_STATE->twoMaps == 1) {
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(SECOND_MAP);
      saved027c = data_027c027c;
      Screen_SetCurrent(SECOND_MAP);
      SetSlotController(i, 1);
      MAP_STATE->army[i + 1] = gMenuState.setup.armyColours[i];
      MAP_STATE->team[i + 1] = gMenuState.setup.teams[i];
      if (i >= gMenuState.setup.unk_2e3) {
        MAP_STATE->slotControllers[i + 1] = PLAYER_CONTROLLER_NONE;
      }
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }

  ApplyWeatherSetting();

  if (MAP_STATE->twoMaps != 1) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(SECOND_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(SECOND_MAP);
  ApplyWeatherSetting();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

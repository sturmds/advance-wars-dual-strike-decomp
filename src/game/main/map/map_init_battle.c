#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/screen.h"
#include "game/screen_other_map.h"

void EventScript_Init(void);
void func_02080734(void);
void Map_StartBattleTask(void);
void Map_SaveCoPicks(void);
BOOL Map_IsCoSelectPhase(void);
void func_020f368c(void);
void func_020f3ee4(void);
void func_020f4098(void);
void Screens_SelectConfigs(void);
void Map_LoadHeaderSetup(int mapId);

/* data_027c0288 holds two game states, one per map. A battle with a second
 * map (header variantMapIds[0] nonzero) uses both; data_027c0284 picks the current one.
 * Screen_SetCurrentGfx and Screen_SetCurrent load a state into data_027c0280 and
 * data_027c027c. */
#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Sets up a battle's maps: clears unk_30 in both game states, takes the
 * second map's id from the current map's header (variantMapIds[0], 0 for none),
 * and copies armies, teams and fog from each map's header. */
void Map_InitBattle(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_02080440_48* game;
  u32 otherUnk30;
  int i;

  EventScript_Init();

  switch (MAP_STATE->gameMode) {
    case 2:
      Map_SaveCoPicks();
      break;
    case 0:
    case 1:
    case 3:
      if (!Map_IsCoSelectPhase()) {
        Map_SaveCoPicks();
      }
      break;
  }

  SWITCH_TO(MAIN_MAP);
  MAP_STATE->unk_1c4->unk_30 = 0;
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  MAP_STATE->unk_1c4->unk_30 = 0;
  SWITCH_BACK();

  Scene_SetKind(0);

  game = MAP_STATE;
  switch (game->gameMode) {
    case 0:
      func_020f4098();
      MAP_STATE->secondMapId =
          Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].variantMapIds[0];
      break;
    case 1:
    case 3:
      game->secondMapId =
          Ov000_gMapHeaders[game->unk_1c4->mapId].variantMapIds[0];
      if (MAP_STATE->secondMapId != 0) {
        SWITCH_TO(OTHER_MAP);
        otherUnk30 = MAP_STATE->unk_1c4->unk_30;
        SWITCH_BACK();
        if (otherUnk30 == 0) {
          Scene_SetKind(4);
        }
      }
      break;
    case 2:
      if (game->secondMapId != 0) {
        SWITCH_TO(OTHER_MAP);
        otherUnk30 = MAP_STATE->unk_1c4->unk_30;
        SWITCH_BACK();
        if (otherUnk30 == 0) {
          Scene_SetKind(4);
        }
      }
      break;
  }

  if (Scene_GetKind() == 3) {
    MAP_STATE->unk_86 = 1;
  }
  func_020f3ee4();

  SWITCH_TO(MAIN_MAP);
  Map_LoadHeaderSetup(MAP_STATE->unk_1c4->mapId);
  SWITCH_BACK();

  if (MAP_STATE->secondMapId != 0) {
    SWITCH_TO(SECOND_MAP);
    /* reads secondMapId of the second state, not the one set above */
    MAP_STATE->unk_1c4->mapId = MAP_STATE->secondMapId;
    Map_LoadHeaderSetup(MAP_STATE->unk_1c4->mapId);
    SWITCH_BACK();
  }

  Screens_SelectConfigs();

  /* On the Auto CO missions the second map's slots keep their players'
     * controllers, so the player can play that front. On other two-map
     * missions the AI plays every used slot of the second map. */
  game = MAP_STATE;
  if (game->gameMode == GAME_MODE_CAMPAIGN && game->secondMapId != 0) {
    if (!Map_IsCoSelectPhase() &&
        (MAP_STATE->unk_1c4->mapId == MAP_LIGHTNING_STRIKES ||
         MAP_STATE->unk_1c4->mapId == MAP_RING_OF_FIRE)) {
      SWITCH_TO(SECOND_MAP);
      for (i = 1; i < ARMY_SLOT_COUNT; i++) {
        MAP_STATE->slotControllers[i] = MAP_STATE->players[i].controller;
      }
      SWITCH_BACK();
    } else {
      SWITCH_TO(SECOND_MAP);
      for (i = 1; i < ARMY_SLOT_COUNT; i++) {
        if (MAP_STATE->slotControllers[i] != PLAYER_CONTROLLER_NONE) {
          MAP_STATE->slotControllers[i] = PLAYER_CONTROLLER_AI;
        }
      }
      SWITCH_BACK();
    }
  }

  Map_StartBattleTask();
  func_020f368c();
  func_02080734();
}

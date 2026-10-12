#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/play_records.h"
#include "game/map_header.h"
#include "game/design_map.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
u8 Player_GetCoId(int player, int slot);
int Co_GetForceRankSetIndex(int co, int player);
void Map_LoadAndCountProperties(int mapId, u8* a, u8* b);
u8 Map_GetLoadedPlayerCount(void);
u8 BuildCoPickList(void);
u8* GetCoPickList(void);
int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int a);
u8 Map_GetPlayerCount(int mapId);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern MenuStateFrom200 data_0229ed30;
extern u32 gLinkPlay;
extern DesignMap data_022a6c84[];

/* data_027c027c->unk_48[data_027c0284], written so that the compiler
 * adds the scaled index before the field offset, as the original does.
 * With the plain form the two loads end up in swapped registers. */
#define GAME                                                                   \
  (*(Unk_02080440_48**)((u8*)data_027c027c +                                   \
                            data_027c0284 * sizeof(Unk_02080440_48*) + (u32) & \
                        ((Unk_02080440*)0)->unk_48))

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define SETUP_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->setup.field)

/* data_027c0288 holds two game states, one per map; see Map_InitBattle. */
#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* Copies the settings of the current battle into the battle setup state
 * (gMenuState): fog, weather, funds, controllers, COs, Force Rank sets,
 * teams and army colours of players 1-4. With unk_2eb clear they come from
 * the map and the stored game rules; with it set, from the players of a
 * game in progress. Reads the main map's state and switches to the second
 * map's state for the second controller of each player. */
void BattleSetup_LoadFromGame(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;
  Unk_027c0280* inner0280;
  Unk_02080440* inner027c;
  BOOL isDesignMap;
  MenuState* state;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(MAIN_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(MAIN_MAP);

  /* Needed to match: some stores go through this pointer and some
     * name gMenuState, and the weather store goes through a u8 pointer.
     * The mix decides which loads the compiler repeats and which
     * registers it picks. */
  gMenuState.setup.fog = GAME->unk_1c4->fog;
  state = &gMenuState;
  state->setup.unk_2dd = GAME->unk_1c4->tileset & PLAYER_CO_ID_MASK;
  state->setup.coEffectsOn = GAME->coEffectsOn;
  state->setup.unk_2de = GAME->dayLimit;
  data_0229ed30.fundsPerProperty = GAME->fundsPerProperty;
  state->setup.unk_2df = GAME->propertyGoal;
  if (gLinkPlay != 0) {
    state->setup.unk_2e1 = 1;
  } else {
    state->setup.unk_2e1 = gPlayRecords.unk_12;
  }
  if (gLinkPlay != 0) {
    gMenuState.setup.unk_2e2 = 2;
  } else {
    gMenuState.setup.unk_2e2 = gPlayRecords.unk_15;
  }

  /* weather setting: 0 when randomWeather is 1, else 1-4 from defaultWeather */
  if (GAME->unk_1c4->randomWeather == 1) {
    *(u8*)&gMenuState.setup.unk_2dc = 0;
  } else {
    switch (GAME->unk_1c4->defaultWeather) {
      case 0:
      default:
        gMenuState.setup.unk_2dc = 1;
        break;
      case 1:
        gMenuState.setup.unk_2dc = 3;
        break;
      case 2:
        gMenuState.setup.unk_2dc = 2;
        break;
      case 3:
        gMenuState.setup.unk_2dc = 4;
        break;
    }
  }

  if (gMenuState.setup.unk_2eb == 0) {
    Map_LoadAndCountProperties(GAME->unk_1c4->mapId, SETUP_FIELD(unk_2e7),
                               SETUP_FIELD(unk_2e8));
    gMenuState.setup.coListCount = BuildCoPickList();
    gMenuState.setup.coList = GetCoPickList();
    gMenuState.setup.unk_2e3 = Map_GetPlayerCount(GAME->unk_1c4->mapId);
    for (i = 0; i < 4; i++) {
      if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
        gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_AI;
      } else {
        gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_HUMAN;
      }
      inner0280 = data_027c0280;
      Screen_SetCurrentGfx(SECOND_MAP);
      inner027c = data_027c027c;
      Screen_SetCurrent(SECOND_MAP);
      if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
        gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_AI;
      } else {
        gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_HUMAN;
      }
      Screen_SetCurrent(inner027c);
      Screen_SetCurrentGfx(inner0280);
      gMenuState.setup.teams[i] = GAME->team[i + 1];
    }
    for (i = 0; i < 4; i++) {
      gMenuState.setup.cos[i][0] =
          GAME->playerCos[i + 1][0] & PLAYER_CO_ID_MASK;
      gMenuState.setup.cos[i][1] =
          GAME->playerCos[i + 1][1] & PLAYER_CO_ID_MASK;
      gMenuState.setup.forceRankSets[i][0] =
          Co_GetForceRankSetIndex(gMenuState.setup.cos[i][0], i + 1);
      gMenuState.setup.forceRankSets[i][1] =
          Co_GetForceRankSetIndex(gMenuState.setup.cos[i][1], i + 1);
    }
    isDesignMap = FALSE;
    if (MAP_IS_CUSTOM(GAME->unk_1c4->mapId)) {
      isDesignMap = TRUE;
    }
    /* needed to match: a byte pointer walking the struct */
    if (isDesignMap) {
      u8* m = (u8*)&gMenuState;
      for (i = 0; i < 4; i++) {
        ((MenuState*)m)->setup.armyColours[0] =
            data_022a6c84[GAME->unk_1c4->mapId - MAP_CUSTOM_FIRST]
                .armyColours[i + 1];
        m++;
      }
    } else {
      u8* m = (u8*)&gMenuState;
      for (i = 0; i < 4; i++) {
        ((MenuState*)m)->setup.armyColours[0] =
            Ov000_gMapHeaders[GAME->unk_1c4->mapId].army[i + 1];
        m++;
      }
    }
  } else {
    gMenuState.setup.unk_2e3 = Map_GetLoadedPlayerCount();
    gMenuState.setup.unk_2e1 = GAME->unk_82;
    gMenuState.setup.unk_2e2 = GAME->unk_8c;
    for (i = 0; i < 4; i++) {
      gMenuState.setup.cos[i][0] = Player_GetCoId(i + 1, 0);
      gMenuState.setup.cos[i][1] = Player_GetCoId(i + 1, 1);
      gMenuState.setup.armyColours[i] = GAME->players[i + 1].armyColour;
      if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
        gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_AI;
      } else {
        gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_HUMAN;
      }
      inner0280 = data_027c0280;
      Screen_SetCurrentGfx(SECOND_MAP);
      inner027c = data_027c027c;
      Screen_SetCurrent(SECOND_MAP);
      if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
        gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_AI;
      } else {
        gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_HUMAN;
      }
      Screen_SetCurrent(inner027c);
      Screen_SetCurrentGfx(inner0280);
      gMenuState.setup.teams[i] = GAME->team[i + 1];
    }
  }

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (Link_IsActive() != 1) {
    gMenuState.setup.cursorButton = 8;
  } else {
    gMenuState.setup.cursorButton =
        (Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) * 2 + 8;
  }
  gMenuState.setup.settingRow = 0;
  gMenuState.setup.unk_2ea = 0;
}

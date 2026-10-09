#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/Unk_02290718.h"
#include "game/map_header.h"
#include "game/design_map.h"

void func_0200ac9c(void *state);
void func_0200acac(void *state);
u8 func_020018a8(int player, int slot);
int func_02017abc(int co, int player);
void func_020c47d0(int mapId, u8 *a, u8 *b);
u8 func_020c3e08(void);
u8 func_020e3478(void);
u8 *GetCoPickList(void);
int func_0207bd80(void);
int func_0207bd6c(void);
int func_020738d0(int a);
u8 Map_GetPlayerCount(int mapId);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern MenuStateFrom200 data_0229ed30;
extern volatile u32 data_0229eaec;
extern Unk_02290718 data_02290718;
extern DesignMap data_022a6c84[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* data_027c0288 holds two game states, one per map; see Map_InitBattle. */
#define MAIN_MAP   (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* Copies the settings of the current battle into the battle setup state
 * (gMenuState): fog, weather, funds, controllers, COs, Force Rank sets,
 * teams and army colours of players 1-4. With unk_2eb clear they come from
 * the map and the stored game rules; with it set, from the players of a
 * game in progress. Reads the main map's state and switches to the second
 * map's state for the second controller of each player. */
void BattleSetup_LoadFromGame(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int i;
    Unk_027c0280 *inner0280;
    Unk_02080440 *inner027c;
    BOOL isDesignMap;
    MenuState *state;

    saved0280 = data_027c0280;
    func_0200acac(MAIN_MAP);
    saved027c = data_027c027c;
    func_0200ac9c(MAIN_MAP);

    /* Some stores go through this pointer and some name gMenuState:
     * the mix changes which loads the compiler repeats (closest shape
     * found so far, 99.5%). */
    state = &gMenuState;
    state->setup.fog = GAME->unk_1c4->fog;
    state->setup.unk_2dd = GAME->unk_1c4->tileset & PLAYER_CO_ID_MASK;
    state->setup.coEffectsOn = GAME->coEffectsOn;
    state->setup.unk_2de = GAME->unk_80;
    data_0229ed30.fundsPerProperty = GAME->fundsPerProperty;
    state->setup.unk_2df = GAME->unk_81;
    if (data_0229eaec != 0) {
        state->setup.unk_2e1 = 1;
    } else {
        state->setup.unk_2e1 = data_02290718.unk_12;
    }
    if (data_0229eaec != 0) {
        gMenuState.setup.unk_2e2 = 2;
    } else {
        gMenuState.setup.unk_2e2 = data_02290718.unk_15;
    }

    /* weather setting: 0 when unk_3b is 1, else 1-4 from unk_3d */
    if (GAME->unk_1c4->unk_3b == 1) {
        state->setup.unk_2dc = 0;
    } else {
        switch (GAME->unk_1c4->unk_3d) {
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
        func_020c47d0(GAME->unk_1c4->mapId, &state->setup.unk_2e7, &state->setup.unk_2e8);
        gMenuState.setup.coListCount = func_020e3478();
        gMenuState.setup.coList = GetCoPickList();
        gMenuState.setup.unk_2e3 = Map_GetPlayerCount(GAME->unk_1c4->mapId);
        for (i = 0; i < 4; i++) {
            if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
                gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_AI;
            } else {
                gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_HUMAN;
            }
            inner0280 = data_027c0280;
            func_0200acac(SECOND_MAP);
            inner027c = data_027c027c;
            func_0200ac9c(SECOND_MAP);
            if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
                gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_AI;
            } else {
                gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_HUMAN;
            }
            func_0200ac9c(inner027c);
            func_0200acac(inner0280);
            gMenuState.setup.teams[i] = GAME->team[i + 1];
        }
        for (i = 0; i < 4; i++) {
            gMenuState.setup.cos[i][0] = GAME->playerCos[i + 1][0] & PLAYER_CO_ID_MASK;
            gMenuState.setup.cos[i][1] = GAME->playerCos[i + 1][1] & PLAYER_CO_ID_MASK;
            gMenuState.setup.forceRankSets[i][0] = func_02017abc(gMenuState.setup.cos[i][0], i + 1);
            gMenuState.setup.forceRankSets[i][1] = func_02017abc(gMenuState.setup.cos[i][1], i + 1);
        }
        isDesignMap = FALSE;
        if (MAP_IS_CUSTOM(GAME->unk_1c4->mapId)) {
            isDesignMap = TRUE;
        }
        /* needed to match: a byte pointer walking the struct */
        if (isDesignMap) {
            u8 *m = (u8 *)&gMenuState;
            for (i = 0; i < 4; i++) {
                ((MenuState *)m)->setup.armyColours[0] = data_022a6c84[GAME->unk_1c4->mapId - MAP_CUSTOM_FIRST].armyColours[i + 1];
                m++;
            }
        } else {
            u8 *m = (u8 *)&gMenuState;
            for (i = 0; i < 4; i++) {
                ((MenuState *)m)->setup.armyColours[0] = Ov000_gMapHeaders[GAME->unk_1c4->mapId].army[i + 1];
                m++;
            }
        }
    } else {
        gMenuState.setup.unk_2e3 = func_020c3e08();
        gMenuState.setup.unk_2e1 = GAME->unk_82;
        gMenuState.setup.unk_2e2 = GAME->unk_8c;
        for (i = 0; i < 4; i++) {
            gMenuState.setup.cos[i][0] = func_020018a8(i + 1, 0);
            gMenuState.setup.cos[i][1] = func_020018a8(i + 1, 1);
            gMenuState.setup.armyColours[i] = GAME->players[i + 1].unk_19;
            if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
                gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_AI;
            } else {
                gMenuState.setup.controllers[i][0] = PLAYER_CONTROLLER_HUMAN;
            }
            inner0280 = data_027c0280;
            func_0200acac(SECOND_MAP);
            inner027c = data_027c027c;
            func_0200ac9c(SECOND_MAP);
            if (GAME->slotControllers[i + 1] == PLAYER_CONTROLLER_AI) {
                gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_AI;
            } else {
                gMenuState.setup.controllers[i][1] = PLAYER_CONTROLLER_HUMAN;
            }
            func_0200ac9c(inner027c);
            func_0200acac(inner0280);
            gMenuState.setup.teams[i] = GAME->team[i + 1];
        }
    }

    func_0200ac9c(saved027c);
    func_0200acac(saved0280);

    if (func_0207bd80() != 1) {
        gMenuState.setup.unk_2e4 = 8;
    } else {
        gMenuState.setup.unk_2e4 = (func_020738d0(func_0207bd6c()) - 1) * 2 + 8;
    }
    gMenuState.setup.unk_2e6 = 0;
    gMenuState.setup.unk_2ea = 0;
}

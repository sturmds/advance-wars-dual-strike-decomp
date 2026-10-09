#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/menu_item.h"

void func_0200ac9c(void *state);
void func_0200acac(void *state);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* data_027c0288 holds two game states, one per front; func_0200acac and
 * func_0200ac9c load one into data_027c0280 and data_027c027c. */
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state)            \
    saved0280 = data_027c0280;      \
    func_0200acac(state);           \
    saved027c = data_027c027c;      \
    func_0200ac9c(state)

#define SWITCH_BACK()               \
    func_0200ac9c(saved027c);       \
    func_0200acac(saved0280)

/* getFlags of the "Auto CO On" battle menu item (gBattleMenuItems). The item is
 * listed only in the Auto CO missions while the other state's unk_30 is 0
 * (probably: the other front is not over),
 * and only when the AI plays that front for the current player. */
s8 BattleMenu_AutoCoOnHidden(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    Unk_02080440_48 *game;
    u32 otherUnk30;
    BOOL hidden;
    u8 player;
    u8 controller;

    game = GAME;
    if (game->gameMode != 0) {
        return MENU_ITEM_HIDDEN;
    }
    if (game->unk_1c4->mapId != MAP_LIGHTNING_STRIKES &&
        game->unk_1c4->mapId != MAP_RING_OF_FIRE) {
        return MENU_ITEM_HIDDEN;
    }

    hidden = TRUE;
    if (game->secondMapId != 0) {
        SWITCH_TO(OTHER_MAP);
        otherUnk30 = GAME->unk_1c4->unk_30;
        SWITCH_BACK();
        if (otherUnk30 == 0) {
            hidden = FALSE;
        }
    }
    if (hidden == TRUE) {
        return MENU_ITEM_HIDDEN;
    }

    player = GAME->currentPlayer;
    SWITCH_TO(OTHER_MAP);
    controller = GAME->players[player].controller;
    SWITCH_BACK();
    return controller != PLAYER_CONTROLLER_AI;
}

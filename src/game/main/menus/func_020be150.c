#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_item.h"
#include "game/screen_other_map.h"

s8 BattleMenu_AutoCoOnHidden(void);
s8 BattleMenu_AutoCoOffHidden(void);

/* data_027c0288 holds two game states, one per front; Screen_SetCurrentGfx and
 * Screen_SetCurrent load one into data_027c0280 and data_027c027c. */
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* getFlags of a battle menu item; name and purpose unknown. Same as
 * func_020be150 with AI behaviour General. */
s8 func_020be150(void) {
  u8 player;
  u8 controller;
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;
  BOOL hidden;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  u32 otherUnk30;

  if (BattleMenu_AutoCoOnHidden() == 0 || BattleMenu_AutoCoOffHidden() == 0) {
    player = MAP_STATE->currentPlayer;
    if (MAP_STATE->players[player].aiBehaviour == AI_BEHAVIOUR_GENERAL) {
      return 0;
    }
  }

  hidden = TRUE;
  if (MAP_STATE->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP, saved0280, saved027c);
    otherUnk30 = MAP_STATE->unk_1c4->unk_30;
    SWITCH_BACK(saved0280, saved027c);
    if (otherUnk30 == 0) {
      hidden = FALSE;
    }
  }
  if (hidden == TRUE) {
    return MENU_ITEM_HIDDEN;
  }

  player = MAP_STATE->currentPlayer;
  SWITCH_TO(data_027c0288.unk_04, savedGfx, savedState);
  controller = MAP_STATE->players[player].controller;
  SWITCH_BACK(savedGfx, savedState);
  if (controller != PLAYER_CONTROLLER_AI) {
    return MENU_ITEM_HIDDEN;
  }
  return MAP_STATE->players[player].aiBehaviour != AI_BEHAVIOUR_GENERAL;
}

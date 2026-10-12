#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_item.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
s8 BattleMenu_AutoCoOnHidden(void);
s8 BattleMenu_AutoCoOffHidden(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

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
 * func_020be358 with AI behaviour Assault. */
s8 func_020be358(void) {
  u8 player;
  u8 controller;
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;
  BOOL hidden;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  u32 otherUnk30;

  if (BattleMenu_AutoCoOnHidden() == 0 || BattleMenu_AutoCoOffHidden() == 0) {
    player = GAME->currentPlayer;
    if (GAME->players[player].aiBehaviour == AI_BEHAVIOUR_ASSAULT) {
      return 0;
    }
  }

  hidden = TRUE;
  if (GAME->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP, saved0280, saved027c);
    otherUnk30 = GAME->unk_1c4->unk_30;
    SWITCH_BACK(saved0280, saved027c);
    if (otherUnk30 == 0) {
      hidden = FALSE;
    }
  }
  if (hidden == TRUE) {
    return MENU_ITEM_HIDDEN;
  }

  player = GAME->currentPlayer;
  SWITCH_TO(data_027c0288.unk_04, savedGfx, savedState);
  controller = GAME->players[player].controller;
  SWITCH_BACK(savedGfx, savedState);
  if (controller != PLAYER_CONTROLLER_AI) {
    return MENU_ITEM_HIDDEN;
  }
  return GAME->players[player].aiBehaviour != AI_BEHAVIOUR_ASSAULT;
}

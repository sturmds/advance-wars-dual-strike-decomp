#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link message 15: a player who is not ready moves one place later in
 * their row's list of members, swapping with the player after them. The
 * last member stays where they are. */
void CombatSetup_MovePlayerDown(int player) {
  int index;
  int i;
  u8 next;
  u8 ready = MENU->combat.players[player].ready;
  u8 row = MENU->combat.players[player].row;

  if (ready != 0) {
    return;
  }
  index = row - 1;
  for (i = 0; i < MENU->combat.rows[index].memberCount; i++) {
    if (player == MENU->combat.rows[index].members[i]) {
      break;
    }
  }
  if (i >= MENU->combat.rows[index].memberCount - 1) {
    return;
  }
  next = MENU->combat.rows[index].members[i + 1];
  MENU->combat.rows[index].members[i + 1] = player;
  MENU->combat.rows[index].members[i] = next;
}

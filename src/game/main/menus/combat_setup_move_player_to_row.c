#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(u32 soundId);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link message 11 and the row moves: a player who is not ready leaves their
 * row's member list (the later members move up one place) and joins the end
 * of row newRow (1-4), unless it already has 7 members. */
void CombatSetup_MovePlayerToRow(int player, u8 newRow) {
  int i;
  int index;
  u8 ready = MENU->combat.players[player].ready;
  u8 row = MENU->combat.players[player].row;

  if (ready != 0) {
    return;
  }
  if (MENU->combat.rows[newRow - 1].memberCount == 7) {
    return;
  }
  index = row - 1;
  for (i = 0; i < MENU->combat.rows[index].memberCount; i++) {
    if (player == MENU->combat.rows[index].members[i]) {
      break;
    }
  }
  for (; i < MENU->combat.rows[index].memberCount - 1; i++) {
    MENU->combat.rows[index].members[i] =
        MENU->combat.rows[index].members[i + 1];
  }
  MENU->combat.rows[index].memberCount--;
  MENU->combat.rows[newRow - 1]
      .members[MENU->combat.rows[newRow - 1].memberCount] = player;
  MENU->combat.rows[newRow - 1].memberCount++;
  MENU->combat.players[player].row = newRow;
  Sound_PlaySe(0x10002);
}

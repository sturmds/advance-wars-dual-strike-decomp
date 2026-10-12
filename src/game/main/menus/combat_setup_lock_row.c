#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void RowLockMark_Start(int row);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link message 16: the player confirms the row. Sets the row's locked flag and
 * tells the row's script; for the local player, closes the row buttons 20
 * and 21 and the Ready button (id 254). A confirmed row ignores the message. */
void CombatSetup_LockRow(int player) {
  int row = MENU->combat.players[player].row;

  if (MENU->combat.rows[row - 1].locked != 0) {
    return;
  }
  Sound_PlaySe(0x10006);
  MENU->combat.rows[row - 1].locked = 1;
  RowLockMark_Start(row);
  if (player != data_0229fb30.unk_45a) {
    return;
  }
  MenuButton_Close(20);
  MenuButton_Close(21);
  MenuButton_Close(254);
}

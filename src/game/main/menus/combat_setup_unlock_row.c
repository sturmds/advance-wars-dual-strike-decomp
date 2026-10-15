#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void RowLockMark_Remove(int row);

extern const MenuButtonTemplate data_02160c28;
extern const MenuButtonTemplate data_02160e80;
extern const MenuButtonTemplate data_02160d90;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link message 17: the player's row is open for changes again. Clears the
 * row's locked flag (set by CombatSetup_LockRow), tells the row's script, and for the
 * local player brings back the Ready button (id 254) and the row's two
 * buttons 20 and 21, placed in steps of 64 across and 16 up. */
void CombatSetup_UnlockRow(int player) {
  int row = MENU->combat.players[player].row;

  Sound_PlaySe(0x10009);
  MENU->combat.rows[row - 1].locked = 0;
  RowLockMark_Remove(row);
  if (player != data_0229fb30.unk_45a) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160c28, 254);
  row = MENU->combat.players[player].row - 1;
  MenuButton_Add(row * 64 + 16, 96 - row * 16, &data_02160e80, 20);
  row = MENU->combat.players[player].row - 1;
  MenuButton_Add(row * 64 + 16, 96 - row * 16, &data_02160d90, 21);
}

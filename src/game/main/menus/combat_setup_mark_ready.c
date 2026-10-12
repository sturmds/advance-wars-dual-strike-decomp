#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_Close(int id);
void MenuButton_DisableTouch(int id);
void MenuButtons_RemoveSelectable(void);
void RowLockMark_Start(int row);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Ready button of the Combat setup screen (see CombatSetup_CancelReady). */
#define BUTTON_READY 254

/* Link player `player` marks their Combat row ready, once the row has at
 * least rows[].memberCount units. For the local player the ready button and the
 * shop buttons go away. */
void CombatSetup_MarkReady(int player) {
  int row;

  row = MENU->combat.players[player].row;
  if (LINK_ROWS[row - 1].spawnCount < MENU->combat.rows[row - 1].memberCount) {
    return;
  }
  Sound_PlaySe(0x10006);
  MENU->combat.rows[row - 1].locked = 1;
  RowLockMark_Start(row);
  if (player != data_0229fb30.unk_45a) {
    return;
  }
  MenuButton_Close(BUTTON_READY);
  MenuButton_DisableTouch(253);
  MenuButtons_RemoveSelectable();
}

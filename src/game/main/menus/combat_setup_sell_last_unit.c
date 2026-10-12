#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_Close(int id);
void CombatSetup_DrawShop(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Fields of LINK_ROWS[row - 1] that are changed in place. Needed to match:
 * the original adds the field offset before the row's byte offset. */
#define ROW_SPAWN_COUNT(row) ((&data_02160ac0->cellCount)[((row) - 1) * 28])
#define ROW_FUNDS(row) (*(u32*)((u8*)&data_02160ac0->funds + ((row) - 1) * 28))

/* Ready button of the Combat setup screen (see CombatSetup_CancelReady). */
#define BUTTON_READY 254

/* Link player `player` sells the last unit of their Combat row, unless the
 * row is marked ready: the unit's price goes back to the row's funds. */
void CombatSetup_SellLastUnit(int player) {
  u8 row;

  row = MENU->combat.players[player].row;
  if (MENU->combat.rows[row - 1].locked != 0) {
    return;
  }
  if (ROW_SPAWN_COUNT(row) == 0) {
    return;
  }
  ROW_SPAWN_COUNT(row)--;
  switch (LINK_ROWS[row - 1].cells[ROW_SPAWN_COUNT(row)].tile) {
    case 1:
      ROW_FUNDS(row) += 1000;
      break;
    case 3:
      ROW_FUNDS(row) += 3000;
      break;
    case 4:
      ROW_FUNDS(row) += 5000;
      break;
    case 5:
      ROW_FUNDS(row) += 6000;
      break;
  }
  if (row != MENU->combat.players[data_0229fb30.unk_45a].row) {
    return;
  }
  CombatSetup_DrawShop();
  /* The row now has fewer than rows[].memberCount units: it can no longer be marked ready. */
  if (player == data_0229fb30.unk_45a &&
      ROW_SPAWN_COUNT(row) == MENU->combat.rows[row - 1].memberCount - 1) {
    MenuButton_Close(BUTTON_READY);
  }
  Sound_PlaySe(0x10009);
}

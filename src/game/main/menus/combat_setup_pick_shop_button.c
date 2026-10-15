#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void CombatSetup_DrawShopHelp(void);

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link player `player` picks the shop button `button` (22-25) for their
 * Combat row, unless the row is marked ready. The help text changes when
 * the row is the local player's. */
void CombatSetup_PickShopButton(int player, int button) {
  int row;

  row = MENU->combat.players[player].row;
  if (MENU->combat.rows[row - 1].locked != 0) {
    return;
  }
  Sound_PlaySe(0x10002);
  MENU->combat.rows[row - 1].buttonId = button;
  if (row != MENU->combat.players[data_0229fb30.unk_45a].row) {
    return;
  }
  CombatSetup_DrawShopHelp();
}

#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_Add(int x, int y, void* def, int id);
void CombatSetup_DrawShop(void);

extern MenuState gMenuState;
extern u8 data_02160e58[];

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Fields of LINK_ROWS[row - 1] that are changed in place. Needed to match:
 * the original adds the field offset before the row offset, so the rows
 * are indexed from the field of the first row. */
#define ROW_FUNDS(row) \
  (*(u32*)&((Unk_02160ac0_Row*)&data_02160ac0->funds)[(row) - 1])
#define ROW_SPAWN_COUNT(row) \
  (*(u8*)&((Unk_02160ac0_Row*)&data_02160ac0->cellCount)[(row) - 1])

/* Combat setup buttons: 22-25 one per unit on sale; 254 the ready button. */
#define BUTTON_FIRST_UNIT 22
#define BUTTON_READY 254

/* A row holds at most this many units. */
#define ROW_MAX_UNITS 20

/* Link player `player` buys the unit of shop button `button` (22-25) for
 * their Combat row, unless the row is marked ready or full: it costs 1000,
 * 3000, 5000 or 6000 of the row's funds. */
void CombatSetup_BuyUnit(int player, int button) {
  int row;

  row = MENU->combat.players[player].row;
  if (MENU->combat.rows[row - 1].locked != 0) {
    return;
  }
  if (LINK_ROWS[row - 1].spawnCount == ROW_MAX_UNITS) {
    return;
  }
  MENU->combat.rows[row - 1].buttonId = button;
  switch (MENU->combat.rows[row - 1].buttonId - BUTTON_FIRST_UNIT) {
    case 0:
      if (LINK_ROWS[row - 1].funds < 1000) {
        return;
      }
      LINK_ROWS[row - 1].cells[LINK_ROWS[row - 1].spawnCount].tile = 1;
      ROW_FUNDS(row) -= 1000;
      break;
    case 1:
      if (LINK_ROWS[row - 1].funds < 3000) {
        return;
      }
      LINK_ROWS[row - 1].cells[LINK_ROWS[row - 1].spawnCount].tile = 3;
      ROW_FUNDS(row) -= 3000;
      break;
    case 2:
      if (LINK_ROWS[row - 1].funds < 5000) {
        return;
      }
      LINK_ROWS[row - 1].cells[LINK_ROWS[row - 1].spawnCount].tile = 4;
      ROW_FUNDS(row) -= 5000;
      break;
    case 3:
      if (LINK_ROWS[row - 1].funds < 6000) {
        return;
      }
      LINK_ROWS[row - 1].cells[LINK_ROWS[row - 1].spawnCount].tile = 5;
      ROW_FUNDS(row) -= 6000;
      break;
  }
  ROW_SPAWN_COUNT(row)++;
  if (row != MENU->combat.players[data_0229fb30.unk_45a].row) {
    return;
  }
  CombatSetup_DrawShop();
  /* The row just reached rows[].memberCount units: it can be marked ready now. */
  if (player == data_0229fb30.unk_45a &&
      LINK_ROWS[row - 1].spawnCount == MENU->combat.rows[row - 1].memberCount) {
    MenuButton_Add(0, 0, data_02160e58, BUTTON_READY);
  }
  Sound_PlaySe(0x10006);
}

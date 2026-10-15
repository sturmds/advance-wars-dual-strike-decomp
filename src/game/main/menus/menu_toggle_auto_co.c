#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ai/ai_turn_state.h"
#include "game/screen_other_map.h"

/* Same arguments as the menu handler; the help text functions take none. */
typedef void (*MenuHelpFn)(int a0, int a1, u32 flags);

void PopupMenu_Refresh(void);
void BattleMenu_DrawAutoCoOffHelp(int a0, int a1, u32 flags);
void BattleMenu_DrawAutoCoOnHelp(int a0, int a1, u32 flags);

#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

/* Battle menu handler of "Auto CO On" and "Auto CO Off" (gBattleMenuItems +
 * 0x120 and + 0x140): on the other front of a two-map battle, hands the
 * current player's army to the AI or back to the human, then draws the help
 * text of the item now listed. Any other controller value returns with the
 * other screen still current. */
void Menu_ToggleAutoCo(int a0, int a1, u32 flags) {
  u16 player = MAP_STATE->currentPlayer;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  MenuHelpFn help;
  u8* controller;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(OTHER_MAP);
  savedGame = data_027c027c;
  Screen_SetCurrent(OTHER_MAP);
  /* needed to match: the original adds offsetof(controller) before the row offset */
  controller = (u8*)&((Player*)&MAP_STATE->players->controller)[player];
  switch (*controller) {
    case PLAYER_CONTROLLER_HUMAN:
      *controller = PLAYER_CONTROLLER_AI;
      help = BattleMenu_DrawAutoCoOnHelp;
      Player_ResetUnitsAiState((u8)player);
      break;
    case PLAYER_CONTROLLER_AI:
      *controller = PLAYER_CONTROLLER_HUMAN;
      help = BattleMenu_DrawAutoCoOffHelp;
      break;
    default:
      return;
  }
  Screen_SetCurrent(savedGame);
  Screen_SetCurrentGfx(savedGfx);
  help(a0, a1, flags);
  PopupMenu_Refresh();
}

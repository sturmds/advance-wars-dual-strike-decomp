#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/ai/ai_turn_state.h"

void SidePanel_Place(int mode);
void Gfx_LoadPlayerBmap0a0Palette(int player);

/* Handler for game state 14. Picks the state that runs the current player's
 * turn from its controller: 15 for a human, 16 for the AI (with the AI turn
 * step reset to 0), 21 (link actions) for controllers 3 to 6. */
void Game_State14_PickTurnState(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  Gfx_LoadPlayerBmap0a0Palette(MAP_STATE->currentPlayer);
  switch (MAP_STATE->players[MAP_STATE->currentPlayer].controller) {
    case PLAYER_CONTROLLER_NONE:
      break;
    case PLAYER_CONTROLLER_HUMAN:
      MAP_STATE->unk_18 = 15;
      break;
    case PLAYER_CONTROLLER_AI:
      MAP_STATE->unk_18 = 16;
      AiTurn_SetState(0);
      break;
    /* needed to match: one case per controller, not a shared case */
    case 3:
      MAP_STATE->unk_18 = 21;
      break;
    case 4:
      MAP_STATE->unk_18 = 21;
      break;
    case 5:
      MAP_STATE->unk_18 = 21;
      break;
    case 6:
      MAP_STATE->unk_18 = 21;
      break;
  }
}

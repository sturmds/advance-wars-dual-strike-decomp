#include "types.h"
#include "game/Unk_02080440.h"

void Player_PlayTheme(int player);
void MapScript_RunList00(void);

/* Handler for game state 6. After day 1, plays the current player's theme.
 * Then runs map script list 0 and switches to state 7. */
void Game_State6_PlayThemeAndScripts(void) {
  Unk_02080440_48* state = MAP_STATE;

  if (state->day > 1) {
    Player_PlayTheme((u8)state->currentPlayer);
  }
  MapScript_RunList00();
  MAP_STATE->unk_18 = 7;
}

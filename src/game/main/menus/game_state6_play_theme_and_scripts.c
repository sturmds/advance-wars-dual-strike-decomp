#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void Player_PlayTheme(int player);
void MapScript_RunList00(void);

/* Handler for game state 6. After day 1, plays the current player's theme.
 * Then runs map script list 0 and switches to state 7. */
void Game_State6_PlayThemeAndScripts(void) {
  Unk_02080440_48* state = STATE;

  if (state->day > 1) {
    Player_PlayTheme((u8)state->currentPlayer);
  }
  MapScript_RunList00();
  STATE->unk_18 = 7;
}

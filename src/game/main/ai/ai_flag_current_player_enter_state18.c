#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void Player_SetUnk33(int player);
void Game_EnterState18(void);

/* Sets Player.unk_33 on the current player, then Game_EnterState18 switches
 * Unk_02080440_48.unk_18 to 18. */
void Ai_FlagCurrentPlayerEnterState18(void) {
  Player_SetUnk33(STATE->currentPlayer);
  Game_EnterState18();
}

#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020cce44(void);

/* Handler for game state 10: calls func_020cce44, then switches to state 11.
 * The name is a guess; what the call does is unknown. */
void Game_State10_RunStep(void) {
  func_020cce44();
  STATE->unk_18 = 11;
}

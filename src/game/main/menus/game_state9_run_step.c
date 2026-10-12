#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020ccd7c(void);

/* Handler for game state 9: calls func_020ccd7c, then switches to state 12.
 * The name is a guess; what the call does is unknown. */
void Game_State9_RunStep(void) {
  func_020ccd7c();
  STATE->unk_18 = 12;
}

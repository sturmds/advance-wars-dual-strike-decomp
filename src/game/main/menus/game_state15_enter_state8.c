#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Last step of MapCursor_ShowRange: runs the hook of the game state at
 * offset 0x3c when set, then moves to Game_State15_RunStep sub-state 8. */
void Game_State15_EnterState8(void) {
  if (HOOKS->unk_3c != NULL) {
    HOOKS->unk_3c();
  }
  GAME->unk_1c = 8;
}

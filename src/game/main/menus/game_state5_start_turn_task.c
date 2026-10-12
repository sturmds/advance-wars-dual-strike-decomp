#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

int SceneMode_IsChanging(void);
BOOL func_020ba2b8(void);
BOOL func_020ba208(void);

/* Handler for game state 5. Switches to state 6, then starts one of the two
 * tasks above (func_020ba2b8 first, else func_020ba208). */
void Game_State5_StartTurnTask(void) {
  if (SceneMode_IsChanging()) {
    return;
  }
  STATE->unk_18 = 6;
  if (func_020ba2b8()) {
    return;
  }
  func_020ba208();
}

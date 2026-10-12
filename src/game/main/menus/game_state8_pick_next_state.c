#include "types.h"
#include "game/Unk_02080440.h"
#include "game/song.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_020fa6f8(void);
void func_020526c8(void);

/* Handler for game state 8. Waits for the song fade to end, then picks the
 * next state: 12 on day 1 from the second player on, 9 after day 1, else 11. */
void Game_State8_PickNextState(void) {
  Unk_02080440_48* state;

  if (Sound_IsSongFadingOut()) {
    return;
  }
  if (func_020fa6f8() != 1 && STATE->day >= 1) {
    func_020526c8();
  }
  state = STATE;
  if (state->day == 1 && state->currentPlayer >= 2) {
    state->unk_18 = 12;
    return;
  }
  if (state->day >= 2) {
    state->unk_18 = 9;
  } else {
    state->unk_18 = 11;
  }
}

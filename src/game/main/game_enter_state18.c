#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Switches unk_18 to 18, remembering the old value in unk_1a. */
void Game_EnterState18(void) {
  if (STATE->unk_18 != 18) {
    STATE->unk_1a = STATE->unk_18;
    STATE->unk_18 = 18;
  }
}

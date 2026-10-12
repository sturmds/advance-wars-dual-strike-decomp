#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Asks the current game state's callback (Unk_02080440_50.unk_5c) for a
 * yes/no answer; FALSE when the state has no callback. */
BOOL func_020fa6f8(void) {
  BOOL (*query)(void) = data_027c027c->unk_50[data_027c0284]->unk_5c;

  if (query == NULL) {
    return FALSE;
  }
  return query();
}

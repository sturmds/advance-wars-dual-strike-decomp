#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_020e8ab0(void) {
  if (data_027c027c->unk_48[data_027c0284]->coSelectPhase == 0) {
    return TRUE;
  }
  return FALSE;
}

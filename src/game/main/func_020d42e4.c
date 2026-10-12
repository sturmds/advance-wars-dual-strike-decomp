#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_020d42e4(void) {
  return data_027c027c->unk_48[data_027c0284]->gameMode == 2;
}

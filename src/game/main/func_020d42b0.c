#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE when the battle's game mode is 3 (see Unk_02080440_48.gameMode). */
BOOL func_020d42b0(void) {
  return data_027c027c->unk_48[data_027c0284]->gameMode == 3;
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/visual_mode.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

u8 func_020c019c(void);
int func_020c0144(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_020bfed4(void) {
  if (func_020c019c() == 1) {
    return TRUE;
  }
  if (func_020c0144() == 0 && STATE->visualMode == VISUAL_D) {
    return FALSE;
  }
  return TRUE;
}
